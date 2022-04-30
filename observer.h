#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>
#include <opencv2/core.hpp>
#include "flags.h"
#include "edits.h"
#include "path.h"
#include "frames.h"
#include "thread_general.h"
//#include "thread_general.h"

using namespace cv;

class Observer
{
public:
    Observer();
    void set_flags(char func, bool value);
    void set_Edits(char func, float* to);
    float* get_Edits(char func);
    void set_Path(std::string* to);
    std::string get_Path();
    bool get_flags(char func);
    void add_Frame(Mat* frame);
    void clear_Frames();
    std::vector<Mat>* get_Frames();
    void start_Analisys();
    void stop_Analisys();
    Mat* get_fire_result();
    Mat* get_smoke_result();
    void clear_thread();

private:
    //класс хранения флагов
    Flags flags;
    //класс хранения введенных параметров
    Edits params;
    //Buffer frames;
    //std::vector<cv::Mat> original;
    Path path;
    Frames buffer;
    thread_General logic;

};

#endif // OBSERVER_H
