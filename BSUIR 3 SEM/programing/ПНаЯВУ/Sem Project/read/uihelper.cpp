#include "uihelper.h"
#include "qscrollbar.h"

uiHelper::uiHelper()
{

}

void uiHelper::nextPage(Ui::Reading *ui)
{
    QScrollBar *verticalScrollBar = ui->textBrowser->verticalScrollBar();
    verticalScrollBar->setValue(verticalScrollBar->value() + ui->textBrowser->viewport()->height());
}

void uiHelper::prevPage(Ui::Reading *ui)
{
    QScrollBar *verticalScrollBar = ui->textBrowser->verticalScrollBar();
    verticalScrollBar->setValue(verticalScrollBar->value() - ui->textBrowser->viewport()->height());
}

void uiHelper::initTextbrowser(Ui::Reading *ui, QString html, int currentPosition)
{

    ui->textBrowser->clear();
    ui->textBrowser->setHtml(html);

    ui->textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->textBrowser->setWordWrapMode(QTextOption::WordWrap);
    ui->textBrowser->verticalScrollBar()->setValue(currentPosition);

}
