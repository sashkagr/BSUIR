/********************************************************************************
** Form generated from reading UI file 'shelveslist.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHELVESLIST_H
#define UI_SHELVESLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ShelvesList
{
public:
    QPushButton *exitButton;
    QPushButton *addShelfBtn;
    QListWidget *listWidget;

    void setupUi(QDialog *ShelvesList)
    {
        if (ShelvesList->objectName().isEmpty())
            ShelvesList->setObjectName("ShelvesList");
        ShelvesList->resize(901, 725);
        exitButton = new QPushButton(ShelvesList);
        exitButton->setObjectName("exitButton");
        exitButton->setGeometry(QRect(769, 20, 101, 24));
        addShelfBtn = new QPushButton(ShelvesList);
        addShelfBtn->setObjectName("addShelfBtn");
        addShelfBtn->setGeometry(QRect(30, 20, 80, 24));
        listWidget = new QListWidget(ShelvesList);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(20, 61, 861, 641));

        retranslateUi(ShelvesList);

        QMetaObject::connectSlotsByName(ShelvesList);
    } // setupUi

    void retranslateUi(QDialog *ShelvesList)
    {
        ShelvesList->setWindowTitle(QCoreApplication::translate("ShelvesList", "Dialog", nullptr));
        exitButton->setText(QCoreApplication::translate("ShelvesList", "ALL BOOKS", nullptr));
        addShelfBtn->setText(QCoreApplication::translate("ShelvesList", "ADD SHELF", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShelvesList: public Ui_ShelvesList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHELVESLIST_H
