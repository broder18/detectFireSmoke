#ifndef THREAD_GENERAL_H
#define THREAD_GENERAL_H
#include <opencv2/core.hpp>


#include <QThread>
//#include "observer.h"
#include "preframes.h"
#include "thread_smoke.h"
#include "thread_fire.h"
#include "edits.h"
#include "flags.h"
#include "frames.h"
#include <QMutex>

using namespace cv;

class thread_General : public QThread
{
public:
    thread_General();
    //void set_Controller(Observer *addr);
    void run();
    void add_and_split();
    void gaussian();
    void set_frames(std::vector<Mat>* input_frames, Edits* addr, Flags* addr2);
    void set_Refresh();
    void clear();


private:
    //Observer* control;
    PreFrames repository;
    thread_Smoke* smoke;
    Thread_fire* fire;
    Edits* params;
    Flags* flags;
    QMutex mu;
    bool analisys_fire = false;
    bool analisys_smoke = false;
    Frames* frames;
    Mat res_smoke;
    Mat res_fire;



};

#endif // THREAD_GENERAL_H
