#include "flags.h"

Flags::Flags()
{

}

void Flags::set_start(bool value)
{
    _start = value;
    //QString sas = QString::number(_start);
    //qDebug() << sas;
}

bool Flags::get_start()
{
    return _start;
}

void Flags::set_pause(bool value)
{
    _pause = value;
}

bool Flags::get_pause()
{
    return _pause;
}

void Flags::set_params(bool value)
{
    _params = value;
    /*QString sas = QString::number(_params);
    qDebug() << sas;*/
}

bool Flags::get_params()
{
    return _params;
}


void Flags::set_analysis(bool value)
{
    _analysis = value;
}

bool Flags::get_analysis()
{
    return _analysis;
}

void Flags::set_cam(bool value)
{
    /*QString sas = QString::number(*value) + "usb камера флаг";
    qDebug() << sas;*/
    _cam = value;
    QString sas = QString::number(_cam) + " flags->get_cam";
    qDebug() << sas;

}

bool Flags::get_cam()
{

    return _cam;
}

void Flags::set_ipcam(bool value)
{
    _ipcam = value;
    QString sas = QString::number(_ipcam) + " flags->get_ip";
    qDebug() << sas;
}

bool Flags::get_ipcam()
{

    return _ipcam;
}

void Flags::set_video(bool value)
{
    _video = value;
    QString sas = QString::number(_video) + " flags->get_video";
    qDebug() << sas;
}

bool Flags::get_video()
{

    return _video;
}

void Flags::set_collection(bool value)
{
    _collection = value;
}
bool Flags::get_collection()
{
    return _collection;
}

void Flags::set_find(bool value)
{
    _find = value;
}

bool Flags::get_find()
{
    return _find;
}

void Flags::set_smoke(bool value)
{
    _smoke = value;
}

bool Flags::get_smoke()
{
    return _smoke;
}

void Flags::set_fire(bool value)
{
    _fire = value;
}

bool Flags::get_fire()
{
    return _fire;
}

void Flags::clear()
{
    _start = false;
    _pause = false;
    _params = false;
    //_path = 0;
    //_index = 0;
    _analysis = false;
}
