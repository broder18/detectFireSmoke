#include "mainwindow.h"
#include <QApplication>
#include "observer.h"
//#include "thread_general.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //thread_Control analisys;
    Observer control;
    MainWindow window;
    //thread_General logic;
    //control.set_analisys
    //logic.set_Controller(&control);
    window.set_Controller(&control);
    window.setWindowTitle("FireSmoke Demo");
    window.setFixedSize(1000, 800);
    window.show();
    return a.exec();
}
