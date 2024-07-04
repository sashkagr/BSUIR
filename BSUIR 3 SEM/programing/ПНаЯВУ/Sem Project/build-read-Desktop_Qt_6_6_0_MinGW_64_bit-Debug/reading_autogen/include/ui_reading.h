/********************************************************************************
** Form generated from reading UI file 'reading.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_READING_H
#define UI_READING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_Reading
{
public:
    QPushButton *exitButton;
    QTextBrowser *textBrowser;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QLabel *label;
    QLabel *label_2;
    QRadioButton *radioButton_4;
    QRadioButton *radioButton_5;
    QRadioButton *radioButton_6;
    QLabel *label_4;
    QSpinBox *spinBox;
    QRadioButton *radioButton_8;
    QRadioButton *radioButton_9;
    QRadioButton *radioButton_7;

    void setupUi(QDialog *Reading)
    {
        if (Reading->objectName().isEmpty())
            Reading->setObjectName("Reading");
        Reading->resize(912, 661);
        QFont font;
        font.setKerning(true);
        Reading->setFont(font);
        exitButton = new QPushButton(Reading);
        exitButton->setObjectName("exitButton");
        exitButton->setGeometry(QRect(660, 10, 80, 24));
        textBrowser = new QTextBrowser(Reading);
        textBrowser->setObjectName("textBrowser");
        textBrowser->setGeometry(QRect(10, 10, 571, 611));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Times New Roman")});
        font1.setBold(false);
        font1.setItalic(false);
        font1.setKerning(true);
        textBrowser->setFont(font1);
        nextButton = new QPushButton(Reading);
        nextButton->setObjectName("nextButton");
        nextButton->setGeometry(QRect(330, 630, 80, 24));
        prevButton = new QPushButton(Reading);
        prevButton->setObjectName("prevButton");
        prevButton->setGeometry(QRect(190, 630, 80, 24));
        radioButton = new QRadioButton(Reading);
        radioButton->setObjectName("radioButton");
        radioButton->setGeometry(QRect(620, 80, 91, 22));
        radioButton_2 = new QRadioButton(Reading);
        radioButton_2->setObjectName("radioButton_2");
        radioButton_2->setGeometry(QRect(620, 110, 91, 22));
        radioButton_3 = new QRadioButton(Reading);
        radioButton_3->setObjectName("radioButton_3");
        radioButton_3->setGeometry(QRect(620, 140, 91, 22));
        label = new QLabel(Reading);
        label->setObjectName("label");
        label->setGeometry(QRect(620, 50, 71, 31));
        label_2 = new QLabel(Reading);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(620, 190, 111, 16));
        radioButton_4 = new QRadioButton(Reading);
        radioButton_4->setObjectName("radioButton_4");
        radioButton_4->setGeometry(QRect(620, 220, 91, 22));
        radioButton_5 = new QRadioButton(Reading);
        radioButton_5->setObjectName("radioButton_5");
        radioButton_5->setGeometry(QRect(620, 250, 91, 22));
        QFont font2;
        font2.setBold(true);
        font2.setKerning(true);
        radioButton_5->setFont(font2);
        radioButton_6 = new QRadioButton(Reading);
        radioButton_6->setObjectName("radioButton_6");
        radioButton_6->setGeometry(QRect(620, 280, 91, 22));
        QFont font3;
        font3.setItalic(true);
        font3.setKerning(true);
        radioButton_6->setFont(font3);
        label_4 = new QLabel(Reading);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(610, 430, 111, 16));
        spinBox = new QSpinBox(Reading);
        spinBox->setObjectName("spinBox");
        spinBox->setGeometry(QRect(680, 430, 42, 25));
        spinBox->setMinimum(8);
        spinBox->setMaximum(25);
        spinBox->setValue(14);
        radioButton_8 = new QRadioButton(Reading);
        radioButton_8->setObjectName("radioButton_8");
        radioButton_8->setGeometry(QRect(620, 350, 91, 22));
        radioButton_9 = new QRadioButton(Reading);
        radioButton_9->setObjectName("radioButton_9");
        radioButton_9->setGeometry(QRect(620, 380, 91, 22));
        radioButton_7 = new QRadioButton(Reading);
        radioButton_7->setObjectName("radioButton_7");
        radioButton_7->setGeometry(QRect(620, 320, 141, 22));

        retranslateUi(Reading);

        QMetaObject::connectSlotsByName(Reading);
    } // setupUi

    void retranslateUi(QDialog *Reading)
    {
        Reading->setWindowTitle(QCoreApplication::translate("Reading", "Dialog", nullptr));
        exitButton->setText(QCoreApplication::translate("Reading", "EXIT", nullptr));
        nextButton->setText(QCoreApplication::translate("Reading", ">", nullptr));
        prevButton->setText(QCoreApplication::translate("Reading", "<", nullptr));
        radioButton->setText(QCoreApplication::translate("Reading", "Day", nullptr));
        radioButton_2->setText(QCoreApplication::translate("Reading", "Night", nullptr));
        radioButton_3->setText(QCoreApplication::translate("Reading", "Sand", nullptr));
        label->setText(QCoreApplication::translate("Reading", "Color mode:", nullptr));
        label_2->setText(QCoreApplication::translate("Reading", "Type of font:", nullptr));
        radioButton_4->setText(QCoreApplication::translate("Reading", "Normal", nullptr));
        radioButton_5->setText(QCoreApplication::translate("Reading", "Bold", nullptr));
        radioButton_6->setText(QCoreApplication::translate("Reading", "Italics", nullptr));
        label_4->setText(QCoreApplication::translate("Reading", "Size:", nullptr));
        radioButton_8->setText(QCoreApplication::translate("Reading", "Currier New", nullptr));
        radioButton_9->setText(QCoreApplication::translate("Reading", "Arial", nullptr));
        radioButton_7->setText(QCoreApplication::translate("Reading", "Times New Roman", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Reading: public Ui_Reading {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_READING_H
