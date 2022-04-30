#ifndef FRAMES_H
#define FRAMES_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <opencv2/highgui.hpp>
#include <QDebug>

using namespace cv;

struct Frames
{
public:
    Frames();
    void add_frames(Mat *cap);
    std::vector<Mat>* get_frames();
    void clear_frames();


    Mat* get_fire_result();
    Mat* get_smoke_result();

private:
    std::vector<Mat> original;
    Mat fire;
    Mat smoke;

public slots:
    void add_fire_result(const Mat& value);
    void add_smoke_result(const Mat& value);
};

#endif // FRAMES_H
