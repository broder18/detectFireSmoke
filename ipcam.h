#ifndef IPCAM_H
#define IPCAM_H

#include <QWidget>
#include <observer.h>

namespace Ui {
class IPcam;
}

class IPcam : public QWidget
{
    Q_OBJECT

public:
    explicit IPcam(QWidget *parent = nullptr);
    void set_Controller(Observer* addr);
    ~IPcam();

private slots:
    void on_pushButton_clicked();


private:
    Ui::IPcam *ui;
    Observer* control;
};

#endif // IPCAM_H
