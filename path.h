#ifndef PATH_H
#define PATH_H

#include <string>
//using namespace std;

struct Path
{
public:
    void set_pathCam(std::string* path);
    std::string get_pathCam();

    Path();
private:
    std::string _path = "none";
};

#endif // PATH_H
