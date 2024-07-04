#include "readbook.h"
#include "ui_readbook.h"

readBook::readBook(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::readBook)
{
    ui->setupUi(this);
}

readBook::~readBook()
{
    delete ui;
}
void readBook::displayBook(const QString &content) {
    ui->textBrowser->clear();
    ui->textBrowser->insertHtml(content);
}


void readBook::on_nextButton_clicked()
{

}


void readBook::on_prevButton_clicked()
{

}

