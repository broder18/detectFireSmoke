#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>


void MainWindow::set_Controller(Observer* addr)
{
    control = addr; 

}

//Метод по изменению флага старт
void MainWindow::change_Start()
{
    char start = '0';
    control->set_flags(start, true);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    timer_u = new QTimer(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


//Открытие формы с настройками
void MainWindow::on_settings_Button_clicked()
{
    Settings *frm = new Settings();
    frm->setAttribute(Qt::WA_ShowModal, true);
    frm->set_Controller(control);
    frm->show();
}

//Старт считывания видео
void MainWindow::on_pushButton_clicked()
{
    char func = '3';
    if(control->get_flags(func) == true)
    {
        QString sas = QString::fromStdString(control->get_Path());
        qDebug() << sas;
        cap.open(std::stoi(control->get_Path()));
    }
    else if (control->get_Path()!="none")
    {
        cap.open(control->get_Path());
    }



    if(!cap.isOpened())
    {
        QMessageBox::warning(this, "Ошибка", "Нет данных об источнике ввода!");
    }
    else
    {
        char start = '0';
        control->set_flags(start, true);
        connect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
        timer->start(40);
    }

}

void MainWindow::update_window()
{

    //Копируем videocapture в mat
    cap >> frame;

    if(frame.empty()) //Если кадр пуст, вызвать метод stopped_video
    {
        stopped_video();
        return;
    }

    //Если флаг collection, отвечающий за сбор данных true, то начать собирать кадры
    if(control->get_flags(collection) == true)
    {
        add_frames();
    }

    cv::resize(frame, frame, Size(900, 600), 0, 0, INTER_CUBIC);//Изменение размера видео для возможности вывода на графический интерфейс

    cvtColor(frame, frame, 4);
    qt_image = QImage((const unsigned char*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
    ui->mainFrame_Label->setPixmap(QPixmap::fromImage(qt_image));
    count++;

}

void MainWindow::update_analisys()//При запуске анализа с определенной периодичностью получаем значение флага analisys
{


   if(control->get_flags(analisys) != true)//При True, timer disconnect и обновляется счетчик. Также флаг collection отвечающий за сбор кадров, переходит в состояние true.
   {
       disconnect(timer_u, SIGNAL(timeout()), this, SLOT(update_analisys()));
       refresh_count();
       collection = true;
   }

   if(control->get_flags('8'))//Если анализ что то нашел
    {


        if(control->get_flags('9'))//Если анализ нашел пламя
        {

            Mat *res_fire = control->get_fire_result();
            result_form *frm_res = new result_form();
            frm_res->setAttribute(Qt::WA_ShowModal, true);
            frm_res->setWindowTitle("Найденное пламя");
            qDebug()<<"Начало ресайза пламени ";
            cv::resize(*res_fire, *res_fire, Size(900, 600), 0, 0, INTER_CUBIC);
            qDebug()<<"Переход к QIamge";
            QImage qt_fire_image = QImage((const unsigned char*)((*res_fire).data), (*res_fire).cols, (*res_fire).rows, QImage::Format_RGB888);
            frm_res->set_frame(&qt_fire_image);
            frm_res->show();

        }

        if(control->get_flags('a'))//Дым
        {
            Mat *res_smoke = control->get_smoke_result();
            result_form *frm_res = new result_form();
            frm_res->setAttribute(Qt::WA_ShowModal, true);
            frm_res->setWindowTitle("Найденный дым");
            cv::resize(*res_smoke, *res_smoke, Size(900, 600), 0, 0, INTER_CUBIC);
            QImage qt_smoke_image = QImage((const unsigned char*)((*res_smoke).data), (*res_smoke).cols, (*res_smoke).rows, QImage::Format_RGB888);
            frm_res->set_frame(&qt_smoke_image);
            frm_res->show();
        }
        control->set_flags('8', false);
        stopped_video();
    }
}

void MainWindow::add_frames()
{
    if(count > *(control->get_Edits(number)))//Если счетчик больше введеного количества кадров установить флаг collection в состояние false
    {
       control->set_flags(collection, false);//Перевод флага сборки кадров в отрицательное состояние
       control->clear_Frames();//Переместить в observer данный метод ??
    }
    else
    {

        if(count == *(control->get_Edits(number)))//Если количество набранных кадров == указанному при вводе начать анализ
        {
            QString sas ="Запуск потока анализа";
            qDebug() << sas;
            control->start_Analisys();//Запуска анализа
            connect(timer_u, SIGNAL(timeout()), this, SLOT(update_analisys()));
            timer_u->start(100);//Установка значения на таймер получения доступа к флагу анализа
            qDebug() << "опять";
        }
        else
        {
            control->add_Frame(&frame);//Добавить кадры в observer
            QString sas = QString::number(count);
            qDebug() << sas;
        }

    }
}

void MainWindow::refresh_count()//Обновление счетчика кадров
{
    count = 0;
}


//Остановка воспроизведения
void MainWindow::on_pushButton_2_clicked()
{
    stopped_video();
}

void MainWindow::stopped_video() //Метод остановки видео
{
    //control->set_flags(&collection, &value_f);
    char start = '0';
    control->set_flags(start, false);//Перевод флага start на false
    disconnect(timer, SIGNAL(timeout()), this, SLOT(update_window()));//Дискоеннект таймера обновления GUI
    ui->mainFrame_Label->clear();//Очистка Label с выводом видео
    frame.release();//Освобождение переменной с кадром
    cap.release();//Освобождение переменной типа videocapture
    control->stop_Analisys();//Дождаться остановки потока анализа
    control->clear_thread();//Очистить все данные из потока анализа
    control->clear_Frames();//Очистить все сохраненные кадры из буфера
}

