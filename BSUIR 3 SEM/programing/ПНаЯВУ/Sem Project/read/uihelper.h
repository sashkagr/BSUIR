#ifndef UIHELPER_H
#define UIHELPER_H
#include "reading.h"
#include <ui_reading.h>

class uiHelper
{
private slots:
    void on_fontStyleChanged();

public:
    uiHelper();
    static void nextPage( Ui::Reading *ui);
    static void prevPage(Ui::Reading *ui);
    static void initTextbrowser(Ui::Reading *ui,QString html, int currentPosition);
};

#endif // UIHELPER_H
