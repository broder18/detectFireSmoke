#ifndef UPDATE_FLAG_H
#define UPDATE_FLAG_H
#include <QThread>
#include "flags.h"
#include <QTimer>

class Update_flag : public QThread
{
public:
    Update_flag();
    void set_path_flags(Flags* obj);
    void run();

private:
    Flags* _path;
private slots:
    void update();

};

#endif // UPDATE_FLAG_H
