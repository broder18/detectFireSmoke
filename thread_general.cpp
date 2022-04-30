#include "thread_general.h"
#include "mainwindow.h"
#include <QDebug>


/*void thread_General::set_Controller(Observer* addr)
{
    //control = addr;
}*/

thread_General::thread_General()
{

}

void thread_General::set_Refresh()
{

}

void thread_General::run()
{
    //int size = params->get_gaussian();//Получить размер ядра Гаусса из Edits

    repository.add_gaussian_B(params->get_gaussian());//Запуск предобработки (примение фильтра Гаусса) для разных каналов
    repository.add_gaussian_G(params->get_gaussian());
    repository.add_gaussian_R(params->get_gaussian());

    smoke = new thread_Smoke(&mu);
    smoke->set_frame(repository.get_frame());
    smoke->set_refB(repository.get_refB());//Передать второму потоку ссылки на объект с файлами предобработки
    smoke->set_refGausG(repository.get_refGausG());
    smoke->set_refGausR(repository.get_refGausR());
    smoke->set_koeff(repository.get_kSmGr(), repository.get_kSmR());
    smoke->set_binar_factor(repository.get_binar_factor());
    smoke->set_guard_interval(repository.get_guard_interval_smoke());
    smoke->set_ref_Normalize(repository.get_normalize_subtract());
    smoke->set_ref_smoke_normalize(repository.get_smoke_normalize());
    smoke->set_resolution(repository.get_resolution());

    fire = new Thread_fire(&mu);
    fire->set_frame(repository.get_frame());
    fire->set_refR(repository.get_refR());//Передать второму потоку ссылки на объект с файлами предобработки
    fire->set_refGausG(repository.get_refGausG());
    fire->set_refGausB(repository.get_refGausB());
    fire->set_koeff(repository.get_kFGr(), repository.get_kFB());
    fire->set_binar_factor(repository.get_binar_factor());
    fire->set_guard_interval(repository.get_guard_interval_fire());
    fire->set_smoke_normalize(repository.get_smoke_normalize());
    fire->set_resolution(repository.get_resolution());

    smoke->pre_run();//Выполнение модулей внутрикадровой обработки, межкадровой разности и нормализации без запуска потоков
    fire->pre_run();
    smoke->start();//Запуск потока дыма
    fire->start();//Запуск потока пламени
    smoke->wait();// Пока не завершитя поток  дыма, остальные действия в текущем потоке не начнутся
    fire->wait();//Ожилание потока пламени


    fire->get_find(analisys_fire);//Получение флага результата воспламенения
    smoke->get_find(analisys_smoke);//Получение флага результата задымления

    if(analisys_fire || analisys_smoke)//Если был найден пожар, то сохранить результат в модуле frames
    {
        flags->set_find(true);//Перевод флага find в состояние true
        if(analisys_fire)//Если analisys_fire == true установить флаг fire в модуле flags в состояние true
        {
            flags->set_fire(true);

            emit frames->add_fire_result(fire->get_result());

        }
        else
        {
            flags->set_smoke(true);
            //frames->add_smoke_result(smoke->get_result());
        }

    }
    qDebug()<<"Дым = ";
    qDebug()<< analisys_smoke;
    qDebug()<<"Пламя = ";
    qDebug()<<analisys_fire;


    delete smoke;//Удаление экземпляра Smoke
    delete fire;
    qDebug()<<"Удаление экземпляров дыма и пламени";
    clear();//Очиска кадров предвыборки
    qDebug()<<"Очистка кадров предвыборки";
    flags->set_analysis(false); // Установка флага анализа в ложное состояние
    qDebug()<<"Флаг анализа в ложном состоянии";

}


void thread_General::clear()
{
    repository.clear();//Удалить все файлы в предобработке
    qDebug() << "Освобождение ресурсов" ;
}

//Передача ссылок на экземпляр входных данныхи передача кадров необходимых для обработки
void thread_General::set_frames(std::vector<Mat>* input_frames, Edits* addr, Flags* addr2)
{
    params = addr;
    repository.set_resolution(params->get_resolution());

    repository.add_koeff(params->get_smokeG(), params->get_smokeR(), params->get_fireG(), params->get_fireB());
    repository.add_binar_koeff(params->get_guard_interval_fire(), params->get_guard_interval_smoke());
    repository.add_binar_factor(params->get_binar_factor());
    flags = addr2;
    repository.add_frames(input_frames);
}



