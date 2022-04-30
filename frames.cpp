#include "frames.h"
#include <iostream>



Frames::Frames()
{
    std::vector<Mat> original;
    std::cout<<"Dokole";
};

//Метод для сохранения выборки
void Frames::add_frames(Mat* cap)
{
    Mat elem = *cap;
    original.push_back(elem);
    //imshow("Sas", *cap);
}

void Frames::add_fire_result(const Mat& value)
{
    fire = value.clone();
}

Mat* Frames::get_fire_result()
{
    return &fire;
}

void Frames::add_smoke_result(const Mat& value)
{
    smoke = value.clone();
}

Mat* Frames::get_smoke_result()
{
    return &smoke;
}

//Метод взятия кадров из выборки
std::vector<Mat>* Frames::get_frames()
{
    return &original;
}

//Очистка кадров
void Frames::clear_frames()
{
    original.clear();
    original.shrink_to_fit();
}
