#ifndef CUSTOMAPPLICATION_H
#define CUSTOMAPPLICATION_H
#include "QApplication"
#include <QIcon>

class CustomApplication : public QApplication
{
    Q_OBJECT

public:
    CustomApplication(int &argc, char **argv);

    bool notify(QObject *receiver, QEvent *event) override;

private:
    void setAppIcon();
};

#endif // CUSTOMAPPLICATION_H
