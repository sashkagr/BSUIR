
#include "Drive.h"
#include "Controller.h"

int main(int argc, char* argv[])
{
    Controller control;
    char deviceLetter = control.get_USBPath();
    std::string newName = control.get_NewName();

    Drive d;
    d.format_volume(deviceLetter, newName);

    return 0;
}
