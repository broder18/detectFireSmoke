#include "thread_smoke.h"
#include <QDebug>
#include <iostream>
#include <opencv2/imgproc.hpp>


thread_Smoke::thread_Smoke(QMutex *addr)
{
    mu = addr;
}

thread_Smoke::~thread_Smoke()
{
    //quit();
    wait();
    quit();
}

void thread_Smoke::set_frame(Mat* input)
{
    frame = input;
}

void thread_Smoke::set_refB(std::vector<Mat>* addr)//Получение ссылок на файлы с предобработкой
{
    refB = addr;
    count = (*addr).size() / 2;
}

void thread_Smoke::set_refGausG(std::vector<Mat>* addr)
{
    refGausG = addr;
}

void thread_Smoke::set_refGausR(std::vector<Mat> *addr)
{
    refGausR = addr;
}

void thread_Smoke::set_koeff(float* addr1,float* addr2)
{
    kGr = addr1;
    kR = addr2;
}

void thread_Smoke::set_binar_factor(float *addr)
{
    binar_factor = addr;
}

void thread_Smoke::set_guard_interval(float *addr)
{
    guard_interval = addr;
}

void thread_Smoke::set_ref_Normalize(Mat *addr)
{
    ref_normalize = addr;
}

void thread_Smoke::set_ref_smoke_normalize(Mat *addr)
{
    ref_smoke_narmalize = addr;
}

void thread_Smoke::set_resolution(float *value)
{
    resolution = value;
}

void thread_Smoke::get_find(bool &value)
{
    value = cropped_v;
}

/*const Mat& thread_Smoke::get_result()
{
    return result;
}*/

void thread_Smoke::set_cropped()
{
    cropped_v = !cropped_v;
    qDebug()<<cropped_v;
}


void thread_Smoke::save_image(const std::string* path, const Mat* value)
{
    std::vector<Mat> channels ;
    Mat space = Mat::zeros((*value).rows, (*value).cols, CV_8UC1);

    Mat fin_img;
    channels.push_back(*value);
    channels.push_back(space);
    channels.push_back(space);
    cv::merge(channels, fin_img);
    cv::imwrite(*path , fin_img);

    fin_img.release();
    space.release();
    channels.clear();

}

void thread_Smoke::rect(int x1, int x2, int y1, int y2)
{
    x1_coord = x1;
    width = x2 - x1;
    y1_coord = y1;
    height = y2 - y1;
}

Mat thread_Smoke::crop(Mat* binar, int x1, int x2, int y1, int y2)
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

void thread_Smoke::draw(Mat *cropped, Mat *input_frame)
{
    Mat output = (*input_frame).clone();
    if(cropped_v)
    {
        std::vector<Mat> color;
        Mat space = Mat::zeros((*cropped).rows, (*cropped).cols, CV_8UC1);

        color.push_back(*cropped);
        color.push_back(space);
        color.push_back(space);

        //qDebug()<<"Добавление в вектор";



        Mat fin_color;
        cv::merge(color, fin_color);
        //qDebug()<<"Объединение в изображение";

        cv::bitwise_or(*input_frame, fin_color , output);
        qDebug()<<"Smoke : Наложение маски";

        cv::Rect obj_img(x1_coord, y1_coord, width, height);
        qDebug()<<"Smoke : Создание объекта Rect";
        cv::rectangle(output , obj_img, cv::Scalar(255, 255, 0), 2);
        qDebug()<<"Smoke : Отрисовка выделенной области";

        //space.release();
        qDebug()<<"Smoke : Удаление space";
        //color.clear();
        //qDebug()<<"Удаление вектора";
        //fin_color.release();
        qDebug()<<"Smoke : Освобождение ресурсов";
    }


    std:: string path = "E:/smoke_mask.png";
    //cv::imwrite(path , output);
    qDebug()<<"Smoke : Сохранение изображения";
    //result = output;
    //output.release();

}


void thread_Smoke::pre_run()
{
    intra_frame(refB, refGausG, refGausR, &intra_out);
    Normalization(&intra_out, &normalize_out);
    inter_frame(&maxB, &normalize_out, &inter_sum);
    *ref_smoke_narmalize = inter_sum;

}


void thread_Smoke::run()
{
    mu->lock();
    std::string path = "E:/inter_subtract_smoke.png";
    save_image( &path , ref_smoke_narmalize);
    mu->unlock();

    mu->lock();
    binar(&maxB, count, binar_factor, guard_interval, ref_smoke_narmalize, &binar_frame);
    mu->unlock();


    mu->lock();
    Mat retush = scan(&binar_frame);
    qDebug()<<"Smoke : Закончили сканирование";
    mu->unlock();

    mu->lock();
    draw(&retush, frame);
    qDebug() << "Smoke : Отрисовка объекта";
    //retush.release();
    mu->unlock();

    qDebug()<<"Smoke : Освобождение retush из памяти";
    qDebug() << "С дымом всё";

}



void thread_Smoke::intra_frame(const std::vector<Mat>* ref_blue, const std::vector<Mat>* ref_gausG, const std::vector<Mat>* ref_gausR, std::vector<Mat>* output)//Внутрикадровая обработка путем нахождения разности
{
    std::vector<Mat> blue = *ref_blue;//Разыменовывание ссылок на данные
    std::vector<Mat> gausR = *ref_gausG;
    std::vector<Mat> gausG = *ref_gausR;

    for(unsigned int n=0; n < blue.size(); n++)
    {
        Mat intra;
        Mat presub;
        presub = (*kGr) * gausG[n] + (*kR) * gausR[n];
        cv::subtract(blue[n], presub, intra);
        (*output).push_back(intra);
    }
    blue.clear();
    gausR.clear();
    gausG.clear();

}

void thread_Smoke::Normalization(std::vector<Mat>* input, std::vector<Mat>* output)
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



void thread_Smoke::inter_frame(double* max_binar,  std::vector<Mat>* input, Mat* output )
{

    *output = Mat::zeros((*input)[0].rows, (*input)[0].cols, CV_8UC1);

    for(unsigned int n = 1; n < (*input).size(); n++)
    {
        Mat inter;
        cv::subtract((*input)[n], (*input)[n-1], inter);//вычитание по модулю

        double min, max;

        minMaxIdx(inter, &min, &max);
        if(*max_binar < max)
        {
           *max_binar = max;
        }

        *output += inter;

    }

    std::string path = "E:/inter.png";
    //save_image(&path, output);---------->
    (*input).clear();
}

void thread_Smoke::Normalization_inter( Mat* input, Mat* output)
{
    double min, max;

    Mat test, presub;
    minMaxIdx((*input), &min, &max);
    double delta = max - min;

    cv::subtract(*input, min, presub);
    *output = 255 * ((*input) - min) / delta;

    qDebug() << min;
    qDebug() << max;
    qDebug() << (double)cv::sum(cv::mean(*input))[0];
    qDebug()<< (double)cv::sum(cv::mean(presub))[0];


    std::string path = "E:/inter_normalize.png";
    //save_image( &path , &presub);---------->
    (*input).release();

}


void thread_Smoke::binar(double *max, short int sum, float* bin_input, float* guard_interval, Mat* input_frame, Mat* output_frame)
{
    int porog = *bin_input / sum * (*max);
    porog = porog + porog * (*guard_interval) / 100;
    cv::threshold(*input_frame, *output_frame, porog, 255, THRESH_BINARY);
    std::string path = "E:/binar_smoke.png";
    //save_image(&path, output_frame);------------>
}

Mat thread_Smoke::scan(Mat* input_frame)
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
                       qDebug()<<"Нашли дым";
                       rect(x1, x2, y1, y2);
                       Mat output;
                       output = crop(input_frame, x1, x2, y1, y2);
                       set_cropped();
                       qDebug()<<"Smoke : Вырезали область дыма";

                       /*inrankedId.release();
                       inranked.release();
                       areas.release();
                       height.release();
                       width.release();
                       labels.release();
                       stats.release();
                       centroids.release();*/

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
    qDebug()<<"Не нашли дым";
    Mat output = Mat::zeros((*input_frame).rows, (*input_frame).cols, CV_8UC1);
    return output;

}



