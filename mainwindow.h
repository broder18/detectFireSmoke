#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTimer>
//#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
//#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include "settings.h"
//#include "frames.h"
#include "QDebug"
#include "observer.h"
#include "result_form.h"


using namespace cv;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void set_Controller(Observer* addr);
    void refresh_count();

private slots:

    void on_settings_Button_clicked();

    void on_pushButton_clicked();

    void update_window();

    void on_pushButton_2_clicked();

    void add_frames();

    void stopped_video();

    void update_analisys();



private:
    Ui::MainWindow *ui;

    QTimer *timer;
    QTimer *timer_u;
    VideoCapture cap;


    Mat frame;
    QImage qt_image;

    //Frames *buffer = new Frames();

    //bool _start;
    //bool _pause;
    //bool _params;
    //bool _path;
    //bool _index;
    //bool _analysis;
    //bool frame_collection=false;
    short int count = 0;
    char analisys = '7';
    char number = '4';
    char collection = '6';

    Observer* control;

    void change_Start();


};
#endif // MAINWINDOW_H
