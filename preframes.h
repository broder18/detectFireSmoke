#ifndef PREFRAMES_H
#define PREFRAMES_H
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QMutex>

using namespace cv;

struct PreFrames
{
public:
    PreFrames();
    void set_find(bool value);
    bool get_find();
    void add_frames(std::vector<Mat>* frames);
    void add_gaussian_R(float* size);
    void add_gaussian_G(float* size);
    void add_gaussian_B(float* size);
    void clear();
    void add_koeff(float* Gr_sm, float* kR_sm, float* Gr_f, float* B_f);
    void add_binar_koeff(float* guard_fire, float* guard_smoke);
    void add_binar_factor(float* value);
    void clear_channelB();
    void clear_channelR();
    void clear_gausB();
    void clear_gausG();
    void clear_gausR();
    float* get_binar_factor();
    float* get_guard_interval_fire();
    float* get_guard_interval_smoke();
    std::vector<Mat>* get_refB();
    std::vector<Mat>* get_refR();
    std::vector<Mat>* get_refGausB();
    std::vector<Mat>* get_refGausG();
    std::vector<Mat>* get_refGausR();
    float* get_kSmR();
    float* get_kSmGr();
    float* get_kFB();
    float* get_kFGr();
    void set_smoke_normalize(Mat* addr);
    Mat* get_smoke_normalize();
    Mat* get_normalize_subtract();
    void clear_original();
    void set_resolution(float* value);
    float* get_resolution();
    Mat* get_frame();




private: 

    //void clear_channel();
    //void clear_gaus();
    std::vector<Mat> channel_R;
    std::vector<Mat> channel_G;
    std::vector<Mat> channel_B;
    std::vector<Mat> gaus_R;
    std::vector<Mat> gaus_G;
    std::vector<Mat> gaus_B;
    std::vector<Mat> original;
    float kGr_sm, kR_sm, kGr_f, kB_f;
    float guard_interval_fire, guard_interval_smoke, binar_factor;
    Mat smoke_normalize;
    Mat normalize_subtract;
    float* resolution;
    Mat frame;
    QMutex mu;

    //Mat rgb_channel[3];

};

#endif // PREFRAMES_H
