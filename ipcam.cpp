#include "ipcam.h"
#include "ui_ipcam.h"

IPcam::IPcam(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IPcam)
{
    ui->setupUi(this);
}

IPcam::~IPcam()
{
    delete ui;
}

void IPcam::set_Controller(Observer* addr)
{
    control = addr;
}

void IPcam::on_pushButton_clicked()
{
    std::string addr = ui->lineEdit->text().toStdString();
    control->set_Path(&addr);
}

