#ifndef VIDEO_H
#define VIDEO_H

#include <QWidget>
#include <observer.h>

namespace Ui {
class Video;
}

class Video : public QWidget
{
    Q_OBJECT

public:
    explicit Video(QWidget *parent = nullptr);
    void set_Controller(Observer* addr);
    ~Video();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Observer* control;
    Ui::Video *ui;
};

#endif // VIDEO_H
