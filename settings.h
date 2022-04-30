#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "observer.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    void set_Controller(Observer* addr);
    ~Settings();

private slots:
    void on_pushButton_clicked();

    void on_radioButton_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_2_clicked();

private:
    Ui::Settings *ui;
    //void set_lineEdit();
    void set_ipCam();
    void set_params();
    //bool cam = true;
    //bool ipCam = false;
    //bool video = false;
    bool value_t = true;
    bool value_f = false;

    char params = '2';
    char cam = '3';
    char ip = '4';
    char video = '5';
    Observer* control;
};

#endif // SETTINGS_H
