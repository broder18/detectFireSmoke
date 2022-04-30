#include "usbcam.h"
#include "ui_usbcam.h"

USBcam::USBcam(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::USBcam)
{
    ui->setupUi(this);

    unsigned int i = 0;
    std::vector<int> all_cams;
    while(i<5)
    {
        cam = VideoCapture(i);
        if(cam.isOpened())
        {
            all_cams.push_back(i);
        }
        i++;
    }

    i=0;
    while(i < all_cams.size())
    {
        ui->comboBox->addItem(QString::number(all_cams[i]));
        i++;
    }


}

USBcam::~USBcam()
{
    delete ui;
}

void USBcam::set_Controller(Observer* addr)
{
    control = addr;
}

void USBcam::on_pushButton_clicked()
{

}


void USBcam::on_comboBox_activated(const QString &arg1)
{
    std::string path = arg1.toStdString();
    control->set_Path(&path);
}

