#include "observer.h"



Observer::Observer()
{

}

void Observer::set_flags(char func, bool value)
{
    switch(func)
    {
    case '0': //Установка значения индекса включаемой камеры
        flags.set_start(value);
        break;
    case '1'://Запуск изменения флага pause
        flags.set_pause(value);
        break;
    case '2'://Запуск изменения флага применения параметров
        flags.set_params(value);
        break;
    case '3'://Изменение флага usb камеры
        flags.set_cam(value);
        break;
    case '4'://Изменение флага IP камеры
        flags.set_ipcam(value);
        break;
    case '5'://Изменение флага видео
        flags.set_video(value);
        break;
    case '6':
        flags.set_collection(value);
        break;
    case '7':
        flags.set_analysis(value);
        break;
    case '8':
        flags.set_find(value);
        flags.set_fire(value);
        flags.set_smoke(value);
        break;
    }
}

bool Observer::get_flags(char func)
{
    switch(func)
    {
    case '0': //Получить флаг start
        return flags.get_start();
    case '1'://Получить флаг pause
        return flags.get_pause();
    case '2'://Получить флаг параметров
        return flags.get_params();
    case '3'://Получить флаг usb камеры
        return flags.get_cam();
    case '4'://Получить флаг ip камеры
        return flags.get_ipcam();
    case '5'://Получить флаг видео
        return flags.get_video();
    case '6':
        return flags.get_collection();
    case '7':
        return flags.get_analysis();
    case '8':
        return flags.get_find();
    case '9':
        return flags.get_fire();
    case 'a':
        return flags.get_smoke();

    }
}

void Observer::set_Edits(char func, float *to)
{
    //char comm = *func;
    //char number = *to;
    switch(func)
    {
    case '0'://К-т fireg сохранить
        params.set_fireG(to);
        break;
    case '1'://К-т smokeg сохранить
        params.set_smokeG(to);
        break;
    case '2'://Размер ядра гаусса сохранить
        params.set_gaussian(to);
        break;
    case '3'://Сохранить разрешение окна сканирования
        params.set_resolution(to);
        break;
    case '4'://Сохранить количество кадров в выборке
        params.set_number(to);
        break;
    case '5':
        params.set_refresh_time(to);
        break;
    case '6'://Сохранить ЗИ для огня
        params.set_guard_interval_fire(to);
        break;
    case '7'://Сохранить ЗИ для дыма
        params.set_guard_interval_smoke(to);
        break;
    case '8'://Сохранить к-т бинаризации
        params.set_binar_factor(to);
        break;

    }
}

float* Observer::get_Edits(char func)
{
    //char comm = *func;
    //char number = *to;
    switch(func)
    {
    case '0'://К-т fireg сохранить
        return params.get_fireG();
    case '1'://К-т smokeg сохранить
        return params.get_smokeG();
    case '2'://Размер ядра гаусса сохранить
        return params.get_gaussian();
    case '3'://Сохранить разрешение окна сканирования
        return params.get_resolution();
    case '4'://Сохранить количество кадров в выборке
        return params.get_number();
    case '5':
        return params.get_refresh_time();
    case '6'://Сохранить ЗИ для огня
        return params.get_guard_interval_fire();
    case '7'://Сохранить ЗИ для дыма
        return params.get_guard_interval_smoke();
    case '8'://Сохранить к-т бинаризации
        return params.get_binar_factor();

    }
}

//Установить путь к выводимому файлу(камере, ip-адрес)
void Observer::set_Path(std::string *to)
{
    path.set_pathCam(to);
}

std::string Observer::get_Path()//Получить путь
{
    return path.get_pathCam();
}

void Observer::add_Frame(Mat* frame)//Добавить кадры выборки
{
    buffer.add_frames(frame);
}

std::vector<Mat>* Observer::get_Frames()//Получить выборку кадров
{
    return buffer.get_frames();
}

void Observer::clear_Frames()//Очистить буффер от выборки
{
    buffer.clear_frames();
}

void Observer::start_Analisys()//Запуск потока с нализом видео
{
    bool value = true;
    set_flags('7', value);
    logic.set_frames(get_Frames(), &params, &flags);
    clear_Frames();
    logic.start();
}

void Observer::stop_Analisys()
{
    logic.wait();
}

Mat* Observer::get_fire_result()
{
    return buffer.get_fire_result();
}

Mat* Observer::get_smoke_result()
{
    return buffer.get_smoke_result();
}

void Observer::clear_thread()//Очистить поток от хранящихся кадров
{
    logic.clear();
}






