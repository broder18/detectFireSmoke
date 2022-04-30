#include "preframes.h"


PreFrames::PreFrames()
{

}

void PreFrames::add_frames(std::vector<Mat>* frames)
{
    //unsigned char buffer[height * width];
    original = *frames;
    for(unsigned int i=0; i<original.size(); i++)
    {
        Mat all[3];
        split(original[i], all);
        channel_B.push_back(all[0]);
        channel_G.push_back(all[1]);
        channel_R.push_back(all[2]);
    }
    original.erase(original.begin(), original.end() - 1);
    frame = (original[0].clone());
    //split(original, rgb_channel);
}

std::vector<Mat>* PreFrames::get_refB()
{
    //QMutexLocker ml(&mu);
    return &channel_B;
}

std::vector<Mat>* PreFrames::get_refR()
{
    //QMutexLocker ml(&mu);
    return &channel_R;
}

std::vector<Mat>* PreFrames::get_refGausG()
{
    //QMutexLocker ml(&mu);
    return &gaus_G;
}

std::vector<Mat>* PreFrames::get_refGausR()
{
    //QMutexLocker ml(&mu);
    return &gaus_R;
}

std::vector<Mat>* PreFrames::get_refGausB()
{
    //QMutexLocker ml(&mu);
    return &gaus_B;
}

void PreFrames::add_gaussian_B(float* size)
{
    for(unsigned int i=0; i < channel_B.size(); i++)
    {
        Mat dst;
        GaussianBlur(channel_B[i], dst, Size(*size, *size), 0, 0);
        gaus_B.push_back(dst);
    }

}

void PreFrames::add_gaussian_G(float* size)
{
    for(unsigned int i=0; i < channel_G.size(); i++)
    {
        Mat dst;
        GaussianBlur(channel_G[i], dst, Size(*size, *size), 0, 0);
        gaus_G.push_back(dst);
    }
    channel_G.clear();
    channel_G.shrink_to_fit();
}

void PreFrames::add_gaussian_R(float* size)
{
    for(unsigned int i=0; i < channel_R.size(); i++)
    {
        Mat dst;
        GaussianBlur(channel_R[i], dst, Size(*size, *size), 0, 0);
        gaus_R.push_back(dst);
    }
}

void PreFrames::clear_original()
{
    original.clear();
    //original.shrink_to_fit();
}

void PreFrames::clear_channelR()
{    
    channel_R.clear();
    //channel_R.shrink_to_fit();
}

void PreFrames::clear_channelB()
{
    channel_B.clear();
    //channel_B.shrink_to_fit();
}

void PreFrames::clear_gausB()
{
    gaus_B.clear();
    //gaus_B.shrink_to_fit();
}

void PreFrames::clear_gausR()
{
    gaus_R.clear();
    //gaus_R.shrink_to_fit();
}

void PreFrames::clear_gausG()
{
    gaus_G.clear();
    //gaus_G.shrink_to_fit();
}

void PreFrames::add_koeff(float* Gr_sm, float* R_sm, float* Gr_f, float* B_f)
{
    kGr_sm = *Gr_sm;
    kR_sm = *R_sm;
    kGr_f = *Gr_f;
    kB_f = *B_f;
}

void PreFrames::add_binar_koeff(float* guard_fire, float* guard_smoke)
{
    guard_interval_fire = *guard_fire;
    guard_interval_smoke = *guard_smoke;
}

void PreFrames::add_binar_factor(float* value)
{
    binar_factor = *value;
}

float* PreFrames::get_guard_interval_fire()
{
    return &guard_interval_fire;
}

float* PreFrames::get_binar_factor()
{
    return &binar_factor;
}

float* PreFrames::get_guard_interval_smoke()
{
    return &guard_interval_smoke;
}

float* PreFrames::get_kSmR()
{
    return &kR_sm;
}

float* PreFrames::get_kSmGr()
{
    return &kGr_sm;
}

float* PreFrames::get_kFB()
{
    return &kB_f;
}

float* PreFrames::get_kFGr()
{
    return &kGr_f;
}


Mat* PreFrames::get_smoke_normalize()
{
    //QMutexLocker ml(&mu);
    return &smoke_normalize;
}


Mat* PreFrames::get_normalize_subtract()
{
    //QMutexLocker ml(&mu);
    return &normalize_subtract;
}

void PreFrames::set_resolution(float* value)
{
    resolution = value;
}

float* PreFrames::get_resolution()
{
    return resolution;
}

Mat* PreFrames::get_frame()
{
    //QMutexLocker ml(&mu);
    return &frame;
}


void PreFrames::clear()
{
    clear_original();
    clear_channelB();
    clear_channelR();
    clear_gausB();
    clear_gausG();
    clear_gausR();

}

