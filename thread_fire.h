#ifndef THREAD_FIRE_H
#define THREAD_FIRE_H

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vector>

#include <QThread>
#include <QMutex>
using namespace cv;

class Thread_fire : public QThread
{
public:
    Thread_fire(QMutex *addr);
    ~Thread_fire();
    void run();
    void pre_run();
    void set_refR(std::vector<Mat>* addr);//Установить ссылку на канал R
    void set_refGausB(std::vector<Mat>* addr);//Установить ссылку на Гаус канала B
    void set_refGausG(std::vector<Mat>* addr);//Установить ссылку на Гаус канала G
    void set_koeff(float* koeff_1, float* koeff_2);//Установить коэффициенты
    void set_binar_factor(float* addr);//Установить к-т бинаризации
    void set_guard_interval(float* addr);//Установить защитный интервал
    void set_smoke_normalize(Mat* addr);
    void set_resolution(float* value);
    void set_frame(Mat* input);
    void get_find(bool& value);
    void set_ref_result(Mat& ref);
    const Mat& get_result();


private:
    void getHist(const cv::Mat& image, const std::string& path);
    void intra_frame(const std::vector<Mat>* red, const std::vector<Mat>* gausG, const std::vector<Mat>* gausB, std::vector<Mat>* output);//Внутрикадровая обработка
    void Normalization(std::vector<Mat>* input, std::vector<Mat>* output);//Нормализация
    void Normalization_inter(Mat* input, Mat* output);
    void inter_frame(double* max_binar, std::vector<Mat>* input, Mat* output);//Межкадровая обработка
    void normalize_subtract(Mat* fire_subtract, Mat* smoke_subtract);
    void binar(double* max, short int sum, float* bin_input, float* guard_interval, Mat* input_frame, Mat* output_frame);//Бинаризация
    Mat scan(Mat* input_frame);
    Mat crop(Mat* binar, int x1, int x2, int y1, int y2);
    void draw(Mat* cropped, Mat* input_frame);
    void rect(int x1, int x2, int y1, int y2);
    void set_cropped();


    void save_image(const std::string* path, const Mat* value);
    QMutex* mu;

    std::vector<Mat>* refR;
    std::vector<Mat>* refGausB;
    std::vector<Mat>* refGausG;
    std::vector<Mat> intra_out;
    std::vector<Mat> normalize_out;
    Mat inter_sum;
    Mat binar_frame;
    float *kGr, *kB;
    float *binar_factor, *guard_interval;
    double maxR;
    Mat* smoke_normalize;
    short int count;
    float* resolution;
    Mat* frame;
    int x1_coord, width, y1_coord, height;
    bool cropped_v = false;
    Mat result;



};
#endif // THREAD_FIRE_H
