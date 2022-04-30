#include "result_form.h"
#include "ui_result_form.h"

result_form::result_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::result_form)
{
    ui->setupUi(this);
}

void result_form::set_frame(QImage *image)
{
    ui->output_label->setPixmap(QPixmap::fromImage(*image));
}

result_form::~result_form()
{
    delete ui;
}
