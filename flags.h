#ifndef FLAGS_H
#define FLAGS_H

//Класс с хранящимися флагами, необходимыми для контроля за исполнениями программы
#include <QDebug>
struct Flags
{
public:
    Flags();
    void set_start(bool value);
    bool get_start();
    void set_pause(bool value);
    bool get_pause();
    void set_params(bool value);
    bool get_params();
    void set_analysis(bool value);
    bool get_analysis();
    void set_collection(bool value);
    bool get_collection();

    void set_find(bool value);
    bool get_find();
    void set_fire(bool value);
    bool get_fire();
    void set_smoke(bool value);
    bool get_smoke();

    void set_cam(bool value);
    bool get_cam();
    void set_ipcam(bool value);
    bool get_ipcam();
    void set_video(bool value);
    bool get_video();
    void clear();

private:
    bool _start = false;
    bool _pause = false;
    bool _params = false;
    bool _analysis = false;
    bool _collection = false;
    bool _find = false;
    bool _smoke = false;
    bool _fire = false;

    bool _cam = false;
    bool _ipcam = false;
    bool _video = false;
};

#endif // FLAGS_H
