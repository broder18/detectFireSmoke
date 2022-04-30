#include "settings.h"
#include "ui_settings.h"
#include "ipcam.h"
#include "usbcam.h"
#include "video.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
}

//Передача адреса контроллера
void Settings::set_Controller(Observer* addr)
{
    control = addr;
}

Settings::~Settings()
{
    delete ui;
}

//Изменение флага params
void Settings::on_pushButton_clicked()
{

    control->set_flags(params, true);
    char collection = '6';
    control->set_flags(collection, true);
    if (control->get_flags(cam) == true)
    {
        //QString sas = QString::number(control->get_flags(&cam)) + " settings->params->cam";
        //qDebug() << sas;
        set_params();
    }
    else if (control->get_flags(ip) == true)
    {
        set_params();
    }
    else
    {
        set_params();
    }
    setEnabled(false);

}

void Settings::set_params()
{
    float comm = ui->lineEdit->text().toFloat();
    control->set_Edits('0', &comm);

    comm = ui->lineEdit_2->text().toFloat();
    control->set_Edits('1', &comm);

    comm = ui->lineEdit_3->text().toFloat();
    control->set_Edits('2', &comm);

    comm = ui->lineEdit_4->text().toFloat();
    control->set_Edits('3', &comm);

    comm = ui->lineEdit_5->text().toFloat();
    control->set_Edits('4', &comm);

    comm = ui->lineEdit_6->text().toFloat();
    control->set_Edits('5', &comm);

    comm = ui->lineEdit_7->text().toFloat();
    control->set_Edits('6', &comm);

    comm = ui->lineEdit_8->text().toFloat();
    control->set_Edits('7', &comm);

    comm = ui->lineEdit_9->text().toFloat();
    control->set_Edits('8', &comm);

}


//Переключение на Ip-камеру
void Settings::on_radioButton_clicked()
{

    ui->lineEdit_6->setEnabled(true);


    IPcam *frm = new IPcam();
    frm->setAttribute(Qt::WA_ShowModal, true);
    frm->set_Controller(control);
    frm->show();

    control->set_flags(ip, true);
    control->set_flags(cam, false);
    control->set_flags(video, false);

}

//Переключение на видео
void Settings::on_radioButton_3_clicked()
{
    ui->lineEdit_6->setDisabled(true);
    Video *frm = new Video();
    frm->setAttribute(Qt::WA_ShowModal, true);
    frm->set_Controller(control);
    frm->show();

    control->set_flags(cam, false);
    control->set_flags(video, true);
    control->set_flags(ip, false);
}

//Переключение на usb-камеру
void Settings::on_radioButton_2_clicked()
{
    ui->lineEdit_6->setEnabled(true);
    USBcam *frm = new USBcam();
    frm->setAttribute(Qt::WA_ShowModal, true);
    frm->set_Controller(control);
    frm->show();

    control->set_flags(ip, false);
    control->set_flags(cam, true);
    control->set_flags(video, false);

}

