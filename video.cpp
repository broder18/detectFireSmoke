#include "video.h"
#include "ui_video.h"
#include "QFileDialog"

Video::Video(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Video)
{
    ui->setupUi(this);
}

Video::~Video()
{
    delete ui;
}

void Video::set_Controller(Observer* addr)
{
    control = addr;
}

void Video::on_pushButton_clicked()
{

}


void Video::on_pushButton_2_clicked()
{
    QString path = QFileDialog::getOpenFileName(0, "Открыть", "", "*.mp4 *.AVI");
    std::string tostr = path.toStdString();
    control->set_Path(&tostr);
}

