#ifndef USBCAM_H
#define USBCAM_H

#include <QWidget>
#include "observer.h"
#include "opencv2/videoio.hpp"

using namespace cv;

namespace Ui {
class USBcam;
}

class USBcam : public QWidget
{
    Q_OBJECT

public:
    explicit USBcam(QWidget *parent = nullptr);
    void set_Controller(Observer* addr);
    ~USBcam();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_activated(const QString &arg1);

private:
    VideoCapture cam;
    Observer* control;
    Ui::USBcam *ui;
};

#endif // USBCAM_H
