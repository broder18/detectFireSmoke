#include "thread_control.h"

thread_Control::thread_Control()
{

}

void thread_Control::start(thread_General* addr)
{
    logic = addr;
    logic->start();
}
