#include "customapplication.h"

CustomApplication::CustomApplication(int &argc, char **argv) : QApplication(argc, argv)
{
    setAppIcon();
    setApplicationName("QT Reader");
}

bool CustomApplication::notify(QObject *receiver, QEvent *event)
{
    if (event->type() == QEvent::FileOpen)
    {
        // Set the application icon here
        setAppIcon();
    }
    return QApplication::notify(receiver, event);
}

void CustomApplication::setAppIcon()
{
    setWindowIcon(QIcon("C:/Users/sashk/OneDrive/Pulpit/bonstic/kursach/read/icon.png"));
}
