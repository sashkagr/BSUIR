#include "mainwindow.h"
#include "customapplication.h"
#include <QtWidgets>

int main(int argc, char* argv[]) {
    CustomApplication app(argc, argv);
    MainWindow w;
    w.show();

    return app.exec();
}


