/********************************************************************************
** Form generated from reading UI file 'addshelfdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDSHELFDIALOG_H
#define UI_ADDSHELFDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AddShelfDialog
{
public:
    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *addShelf;
    QPushButton *cancelButton;
    QLabel *nameInUseWarning;

    void setupUi(QWidget *AddShelfDialog)
    {
        if (AddShelfDialog->objectName().isEmpty())
            AddShelfDialog->setObjectName("AddShelfDialog");
        AddShelfDialog->resize(901, 100);
        lineEdit = new QLineEdit(AddShelfDialog);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(170, 20, 631, 24));
        label = new QLabel(AddShelfDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(90, 20, 81, 16));
        addShelf = new QPushButton(AddShelfDialog);
        addShelf->setObjectName("addShelf");
        addShelf->setGeometry(QRect(720, 60, 80, 24));
        cancelButton = new QPushButton(AddShelfDialog);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setGeometry(QRect(610, 60, 80, 24));
        nameInUseWarning = new QLabel(AddShelfDialog);
        nameInUseWarning->setObjectName("nameInUseWarning");
        nameInUseWarning->setGeometry(QRect(130, 70, 431, 16));

        retranslateUi(AddShelfDialog);

        QMetaObject::connectSlotsByName(AddShelfDialog);
    } // setupUi

    void retranslateUi(QWidget *AddShelfDialog)
    {
        AddShelfDialog->setWindowTitle(QCoreApplication::translate("AddShelfDialog", "Form", nullptr));
        label->setText(QCoreApplication::translate("AddShelfDialog", "SHELF NAME:", nullptr));
        addShelf->setText(QCoreApplication::translate("AddShelfDialog", "ADD", nullptr));
        cancelButton->setText(QCoreApplication::translate("AddShelfDialog", "CANCEL", nullptr));
        nameInUseWarning->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AddShelfDialog: public Ui_AddShelfDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDSHELFDIALOG_H
