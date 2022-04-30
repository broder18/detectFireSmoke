#include "thread_fire.h"
#include <QDebug>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


Thread_fire::Thread_fire(QMutex *addr)
{
    mu = addr;
}
Thread_fire::~Thread_fire()
{
    wait();
    quit();
}

/*void Thread_fire::set_ref_result(Mat& ref)
{
    result = ref;
}*/

void Thread_fire::set_frame(Mat* input)
{
    frame = input;
}

void Thread_fire::set_refR(std::vector<Mat>* addr)//Получение ссылок на файлы с предобработкой
{
    refR = addr;
    count = (*addr).size();
}

void Thread_fire::set_refGausG(std::vector<Mat>* addr)
{
    refGausG = addr;
}

void Thread_fire::set_refGausB(std::vector<Mat> *addr)
{
    refGausB = addr;
}

void Thread_fire::set_koeff(float* addr1,float* addr2)
{
    kGr = addr1;
    kB = addr2;
}

void Thread_fire::set_binar_factor(float *addr)
{
    binar_factor = addr;
}

void Thread_fire::set_guard_interval(float *addr)
{
    guard_interval = addr;
}

void Thread_fire::set_smoke_normalize(Mat *addr)
{
    smoke_normalize = addr;
}

void Thread_fire::set_resolution(float* value)
{
    resolution = value;
}

void Thread_fire::get_find(bool& value)
{
    value = cropped_v;
}

const Mat& Thread_fire::get_result()
{
    return result;
}

void Thread_fire::set_cropped()
{
    cropped_v = !cropped_v;
    qDebug()<<cropped_v;
}

void Thread_fire::save_image(const std::string* path, const Mat* value)
{
    std::vector<Mat> channels ;
    Mat space = Mat::zeros((*value).rows, (*value).cols, CV_8UC1);

    Mat fin_img;
    channels.push_back(space);
    channels.push_back(space);
    channels.push_back(*value);
    cv::merge(channels, fin_img);
    cv::imwrite(*path , fin_img);
}

void Thread_fire::rect(int x1, int x2, int y1, int y2)
{
    x1_coord = x1;
    width = x2 - x1;
    y1_coord = y1;
    height = y2 - y1;
}

Mat Thread_fire::crop(Mat* binar, int x1, int x2, int y1, int y2)
{
    Mat retush = Mat::zeros((*binar).rows, (*binar).cols, CV_8UC1);
    for(int i = x1; i < x2+1; i++)
    {
        for(int j = y1; j < y2+1; j++)
        {
            retush.at<uchar>(j, i) = binar->at<uchar>(j, i);
        }
    }

    return retush;
}

void Thread_fire::draw(Mat *cropped, Mat *input_frame)
{
    Mat output = (*input_frame).clone();
    if(cropped_v)
    {
        std::vector<Mat> color;
        Mat space = Mat::zeros((*cropped).rows, (*cropped).cols, CV_8UC1);

        color.push_back(space);
        color.push_back(space);
        color.push_back(*cropped);

        //qDebug()<<"Добавление в вектор";



        Mat fin_color;
        cv::merge(color, fin_color);
        //qDebug()<<"Объединение в изображение";

        cv::bitwise_or(*input_frame, fin_color , output);
        qDebug()<<"Наложение маски";

        cv::Rect obj_img(x1_coord, y1_coord, width, height);
        qDebug()<<"Создание объекта Rect";
        cv::rectangle(output , obj_img, cv::Scalar(0, 255, 255), 2);
        qDebug()<<"Отрисовка выделенной области";

        space.release();
        qDebug()<<"Удаление space";
        color.clear();
        //qDebug()<<"Удаление вектора";
        fin_color.release();
        qDebug()<<"Освобождение ресурсов пламени";
    }


    std:: string path = "E:/fire_mask.png";
    //cv::imwrite(path , output);
    result = output.clone();
    qDebug()<<"Сохранение изображения";
    //output.release();

}

void Thread_fire::getHist(const cv::Mat& image, const std::string& path)
{
    cv::Mat hist = cv::Mat::zeros(1, 256, CV_64FC1);
    for (int i = 0; i < image.cols; i++)
        for (int j =0; j<image.rows; j++)
        {
            int r = image.at<unsigned char>(j, i);
            hist.at<double>(0, r) = hist.at<double>(0, r) + 1.0;
        }

    double m = 0, M = 0;
    minMaxLoc(hist, &m, &M);
    hist = hist / M;
    cv::Mat hist_img = cv::Mat::zeros(110, 300, CV_8U);

    for (int i = 22; i < 278; i++)
        for(int j = 0; j<100; j++)
        {
            if (hist.at<double>(0, i-22) * 100 > j)
            {
                hist_img.at<unsigned char>(99 - j, i) = 255;
            }
        }

    bitwise_not(hist_img, hist_img);
    cv::resize(hist_img, hist_img, cv::Size(hist_img.cols * 4,hist_img.rows * 4), 0, 0, cv::INTER_CUBIC);
    //std::string path = "E:/histogram.png";
    cv::imwrite(path, hist_img);

}


void Thread_fire::pre_run()
{

    intra_frame(refR, refGausG, refGausB, &intra_out);
    Normalization(&intra_out, &normalize_out);
    inter_frame(&maxR, &normalize_out, &inter_sum);
    normalize_subtract(&inter_sum, smoke_normalize);

}


void Thread_fire::run()
{
    mu->lock();
    binar(&maxR, count, binar_factor, guard_interval, &inter_sum, &binar_frame);
    qDebug()<< "Бинаризация огня окончена";
    mu->unlock();

    mu->lock();
    Mat retush = scan(&binar_frame);
    qDebug() << "Закончили сканирование";
    mu->unlock();

    mu->lock();
    draw(&retush, frame);
    mu->unlock();
    //retush.release();
    qDebug() << "С пламенем всё";

}



void Thread_fire::intra_frame(const std::vector<Mat>* ref_red, const std::vector<Mat>* ref_gausG, const std::vector<Mat>* ref_gausB, std::vector<Mat>* output)//Внутрикадровая обработка путем нахождения разности
{
    std::vector<Mat> gausB = *ref_gausB;//Разыменовывание ссылок на данные
    std::vector<Mat> red = *ref_red;
    std::vector<Mat> gausG = *ref_gausG;

    for(unsigned int n=0; n < red.size(); n++)
    {
        Mat intra;
        Mat presub;
        presub = (*kGr) * gausG[n] + (*kB) * gausB[n];
        cv::subtract(red[n], presub, intra);
        (*output).push_back(intra);
    }

    red.clear();
    gausB.clear();
    gausG.clear();    


}

void Thread_fire::Normalization(std::vector<Mat>* input, std::vector<Mat>* output)
{
    for(unsigned int i = 0; i<(*input).size(); i++)
    {
        Mat normalize;
        double min, max;
        minMaxIdx((*input)[i], &min, &max);
        double delta = max - min;

        normalize = 255 * ((*input)[i] - min) / delta;
        (*output).push_back(normalize);

    }

    (*input).clear();
}



void Thread_fire::inter_frame(double* max_binar, std::vector<Mat>* input, Mat* output)
{

    *output = Mat::zeros((*input)[0].rows, (*input)[0].cols, CV_8UC1);

    for(unsigned int n = 1; n < (*input).size(); n++)
    {
        Mat inter;

        absdiff((*input)[n], (*input)[n-1], inter);//вычитание по модулю
        double min, max;

        minMaxIdx(inter, &min, &max);
        if(*max_binar < max)
        {
           *max_binar = max;
        }
        qDebug() << *max_binar;
        *output += inter;

    }

    std::string path = "E:/inter_fire.png";
    //save_image(&path, output);---------> Сохранение межкадровой

    (*input).clear();
}

void Thread_fire::Normalization_inter(Mat* input, Mat* output)
{
    double min, max;
    minMaxIdx((*input), &min, &max);
    double delta = max - min;
    *output = 255 * ((*input) - min) / delta;
    std::string path = "E:/inter_normalize_fire.png";
    //save_image( &path , output);---------> Сохранение нормализации межкадровой
    (*input).release();
}

void Thread_fire::normalize_subtract(Mat* fire_subtract, Mat* smoke_subtract)
{

    //Mat fire_out =  Mat::zeros((*fire_subtract).rows, (*fire_subtract).cols, CV_8UC1);
    //Mat smoke_out = Mat::zeros((*smoke_subtract).rows, (*smoke_subtract).cols, CV_8UC1);
    double min_sm, min_fire, max_sm, max_fire;

    std::string path_smoke = "E:/histogram_smoke.png";
    std::string path_fire = "E:/histogram_fire.png";

   //getHist(*fire_subtract, path_fire);---------> Получение гистограм
    //getHist(*smoke_subtract, path_smoke);

    minMaxIdx(*fire_subtract, &min_fire, &max_fire);
    minMaxIdx(*smoke_subtract, &min_sm, &max_sm);
    for(int i =0; i<(*fire_subtract).rows;i++)
    {
        for(int j=0; j<(*fire_subtract).cols;j++)
        {
            if(fire_subtract->at<uchar>(i,j)<(0.7 * max_fire))
            {
                //fire_out.at<uchar>(i,j) = 0;
                fire_subtract->at<uchar>(i,j) = 0;
            }
            /*else if (fire_subtract->at<uchar>(i,j)<(0.2 * max_fire))
            {
                fire_subtract->at<uchar>(i,j) = 0;
            }*/

            if (smoke_subtract->at<uchar>(i,j) < (0.1 * max_sm))
            {

                smoke_subtract->at<uchar>(i,j) = 0;
            }
            else if (smoke_subtract->at<uchar>(i,j) > (0.3 * max_sm))
            {
                /*fire_out.at<uchar>(i,j) = (*fire_subtract).at<uchar>(i,j);
                smoke_out.at<uchar>(i,j) = smoke_subtract->at<uchar>(i,j);*/
                smoke_subtract->at<uchar>(i,j) = 0;
            }
        }
    }



    //path_smoke = "E:/histogram_smoke_subtract.png";
    //path_fire = "E:/histogram_fire_subtract.png";

    //getHist(*fire_subtract, path_fire);
    //getHist(*smoke_subtract, path_smoke);



    std::string path = "E:/inter_subtract_fire.png";
    //save_image( &path , fire_subtract);----------->  Сохранение нормализации разностей дыма и пламени
}


void Thread_fire::binar(double *max, short int sum, float* bin_input, float* guard_interval, Mat* input_frame, Mat* output_frame)
{
    int porog = *bin_input / sum * (*max);
    porog = porog + porog * (*guard_interval) / 100;
    cv::threshold(*input_frame, *output_frame, porog, 255, THRESH_BINARY);
    std::string path = "E:/binar_fire.png";
    //getHist(*output_frame);
    //save_image(&path, output_frame);--------> Бинаризация

}


Mat Thread_fire::scan(Mat* input_frame)
{

    Mat labels, stats, centroids;
    cv::connectedComponentsWithStats(*input_frame, labels, stats, centroids, 4);
    Mat areas = stats.col(4).clone();
    Mat height = stats.col(3);
    Mat width = stats.col(2);

    Mat inrankedId, inranked;
    cv::sort(areas, inranked, cv::SORT_EVERY_COLUMN + cv::SORT_DESCENDING);//сортировка площадей по убыванию
    cv::sortIdx(areas, inrankedId,cv::SORT_EVERY_COLUMN + cv::SORT_DESCENDING);//ортировка итераторов в массиве площадей


    int i = 1;

    ushort j = inrankedId.at<ushort>(1, 0);
    while(areas.at<int>(j, 0)> (*resolution * (*resolution)))//Сканировать, пока площадь больше разрешения в квадрате
    {
        if(width.at<int>(j, 0) > (*resolution) && height.at<int>(j, 0) > (*resolution))//Если высота и ширина рассматриваемой зоны больше разрешения окна сканирования
        {
            int x1 = stats.at<int>(j, CC_STAT_LEFT);//координаты левой верхней точки в рассматривемой области
            int y1 = stats.at<int>(j, CC_STAT_TOP);
            int x2 = x1 + width.at<int>(j, 0);//... правой нижней точки ...
            int y2 = y1 + height.at<int>(j, 0);
            int xn = x1;
            while(xn < (x2 - (*resolution)))
            {
                int yn = y1;
                while(yn < (y2 - (*resolution)))
                {
                    int count = 0;
                    for(int k = 0; k < (*resolution); k++)
                    {
                        for(int l = 0; l < (*resolution); l++)
                        {
                            if(input_frame->at<uchar>(yn + k, xn + l) == 255)
                            {
                                count++;
                            }
                        }
                    if(count == (*resolution * (*resolution)))
                    {
                       qDebug()<<"Нашли пламя";
                       rect(x1, x2, y1, y2);
                       Mat output;
                       output = crop(input_frame, x1, x2, y1, y2);
                       set_cropped();

                       /*labels.release();
                       stats.release();
                       centroids.release();
                       areas.release();
                       height.release();
                       width.release();
                       inrankedId.release();
                       inranked.release();*/

                       return output;

                    }
                    }
                    yn++;
                }
                xn++;
            }
        }


        i++;
        j = inrankedId.at<ushort>(i, 0);


    }
    qDebug()<<"Не нашли пламя";
    Mat output = Mat::zeros((*input_frame).rows, (*input_frame).cols, CV_8UC1);

    /*labels.release();
    stats.release();
    centroids.release();
    areas.release();
    height.release();
    width.release();
    inrankedId.release();
    inranked.release();*/

    return output;

}
