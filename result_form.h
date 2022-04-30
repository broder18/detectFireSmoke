#ifndef RESULT_FORM_H
#define RESULT_FORM_H

#include <QWidget>

namespace Ui {
class result_form;
}

class result_form : public QWidget
{
    Q_OBJECT

public:
    explicit result_form(QWidget *parent = nullptr);
    ~result_form();
    void set_frame(QImage *image);

private:
    Ui::result_form *ui;
};

#endif // RESULT_FORM_H
