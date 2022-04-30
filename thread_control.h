#ifndef THREAD_CONTROL_H
#define THREAD_CONTROL_H
#include "thread_general.h"

class thread_Control
{
public:
    thread_Control();
    void start(thread_General* addr);
private:
    thread_General* logic;

#endif // THREAD_CONTROL_H
