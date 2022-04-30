#include "path.h"
#include <QDebug>

Path::Path()
{

}

void Path::set_pathCam(std::string* path)
{
    _path = *path;
    QString sas = QString::fromStdString(_path);
    qDebug() << sas;
}

std::string Path::get_pathCam()
{
    return _path;
}
