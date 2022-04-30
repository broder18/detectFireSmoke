#ifndef COMMUNICATE_H
#define COMMUNICATE_H
#include "mainwindow.h"

class Communicate
{
public:
    Communicate(MainWindow* addr);
    void refresh_analisys();
private:
    MainWindow* obj;
};

#endif // COMMUNICATE_H
