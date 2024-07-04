#include "mainwindow.h"
#include "reading.h"
#include "ui_reading.h"
#include "uihelper.h"
#include "qbuttongroup.h"
#include "customapplication.h"
#include <QScrollBar>




Reading::Reading(QString content,int id,QWidget *parentProxy, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Reading()),
    content(content)
{

    ui->setupUi(this);
    ui->spinBox->setValue(fontSize);
    parentWindow = parentProxy;

    group1.addButton(ui->radioButton);
    group1.addButton(ui->radioButton_2);
    group1.addButton(ui->radioButton_3);

    group2.addButton(ui->radioButton_4);
    group2.addButton(ui->radioButton_5);
    group2.addButton(ui->radioButton_6);

    group3.addButton(ui->radioButton_7);
    group3.addButton(ui->radioButton_8);
    group3.addButton(ui->radioButton_9);

    group1.setExclusive(true);
    ui->radioButton->setChecked(true);

    group2.setExclusive(true);
    ui->radioButton_4->setChecked(true);

    group3.setExclusive(true);
    ui->radioButton_7->setChecked(true);


    connect(ui->radioButton_4, &QRadioButton::toggled, this, &Reading::on_fontStyleChanged);
    connect(ui->radioButton_5, &QRadioButton::toggled, this, &Reading::on_fontStyleChanged);
    connect(ui->radioButton_6, &QRadioButton::toggled, this, &Reading::on_fontStyleChanged);

    connect(ui->radioButton, &QRadioButton::toggled, this, &Reading::on_backgroundColorChanged);
    connect(ui->radioButton_2, &QRadioButton::toggled, this, &Reading::on_backgroundColorChanged);
    connect(ui->radioButton_3, &QRadioButton::toggled, this, &Reading::on_backgroundColorChanged);

    connect(ui->radioButton_7, &QRadioButton::toggled, this, &Reading::on_fontChanged);
    connect(ui->radioButton_8, &QRadioButton::toggled, this, &Reading::on_fontChanged);
    connect(ui->radioButton_9, &QRadioButton::toggled, this, &Reading::on_fontChanged);

    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));


    displayBook(generateStyledHTML(content, fontSize, fontFamily));

    parentProxy->close();
    setWindowTitle(CustomApplication::applicationName());
}

Reading::~Reading()
{
    delete ui;
}


void Reading::on_exitButton_clicked()
{
    parentWindow->show();
    this->close();
}

void Reading::displayBook(QString html) {
    uiHelper::initTextbrowser(ui,html,currentPosition);
}


void Reading::on_prevButton_clicked()
{
    uiHelper::prevPage(ui);

}


void Reading::on_nextButton_clicked()
{
    uiHelper::nextPage(ui);
}

void Reading::on_fontStyleChanged(bool isChecked) {
    if (isChecked) {
         currentPosition = ui->textBrowser->verticalScrollBar()->value();

        ui->textBrowser->clear();

        QFont font = ui->textBrowser->font();

        if (ui->radioButton_4->isChecked()) {
            font.setWeight(QFont::Normal);
            font.setStyle(QFont::StyleNormal);
        } else if (ui->radioButton_5->isChecked()) {
            font.setWeight(QFont::Bold);
        } else if (ui->radioButton_6->isChecked()) {
            font.setStyle(QFont::StyleItalic);
        }

        ui->textBrowser->setFont(font);
        displayBook(generateStyledHTML(content, fontSize, fontFamily));


    }
}

void Reading::on_backgroundColorChanged(bool isChecked) {
    if (isChecked) {
        QPalette palette = ui->textBrowser->palette();

        if (ui->radioButton_2->isChecked()) {
            palette.setColor(QPalette::Base, Qt::black);
            palette.setColor(QPalette::Text, Qt::white);
        } else if (ui->radioButton->isChecked()) {
            palette.setColor(QPalette::Base, Qt::white);
            palette.setColor(QPalette::Text, Qt::black);
        } else if (ui->radioButton_3->isChecked()) {

            palette.setColor(QPalette::Base, QColor(237, 201, 175));
            palette.setColor(QPalette::Text, Qt::black);
        }
        ui->textBrowser->setPalette(palette);
        currentPosition = ui->textBrowser->verticalScrollBar()->value();
        displayBook(generateStyledHTML(content, fontSize, fontFamily));
    }
}

void Reading::on_spinBox_valueChanged(int newValue)
{
    fontSize = newValue;
    currentPosition = ui->textBrowser->verticalScrollBar()->value();
    displayBook(generateStyledHTML(content, fontSize, fontFamily));
}

QString Reading::generateStyledHTML(const QString& styleSheet,int fontSize, QString fontFamily) {
    return styleSheet.arg(fontSize).arg(fontFamily);
}
void Reading::on_fontChanged(bool isChecked)
{
    if (ui->radioButton_7->isChecked()) {
        fontFamily = "Times New Roman";
    } else if (ui->radioButton_8->isChecked()) {
        fontFamily = "Currier New";
    } else if (ui->radioButton_9->isChecked()) {
        fontFamily = "Arial";
    }

    currentPosition = ui->textBrowser->verticalScrollBar()->value();
    displayBook(generateStyledHTML(content, fontSize, fontFamily));
}
