#include "communicate.h"

Communicate::Communicate(MainWindow* addr)
{
    obj = addr;
}

void Communicate::refresh_analisys()
{
    obj->refresh_count();
}
