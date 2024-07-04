/********************************************************************************
** Form generated from reading UI file 'selectbookstoshelf.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTBOOKSTOSHELF_H
#define UI_SELECTBOOKSTOSHELF_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SelectBooksToShelf
{
public:
    QListWidget *listWidget;
    QPushButton *cancelButton;

    void setupUi(QWidget *SelectBooksToShelf)
    {
        if (SelectBooksToShelf->objectName().isEmpty())
            SelectBooksToShelf->setObjectName("SelectBooksToShelf");
        SelectBooksToShelf->resize(901, 725);
        listWidget = new QListWidget(SelectBooksToShelf);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(10, 10, 871, 641));
        cancelButton = new QPushButton(SelectBooksToShelf);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setGeometry(QRect(800, 680, 80, 24));

        retranslateUi(SelectBooksToShelf);

        QMetaObject::connectSlotsByName(SelectBooksToShelf);
    } // setupUi

    void retranslateUi(QWidget *SelectBooksToShelf)
    {
        SelectBooksToShelf->setWindowTitle(QCoreApplication::translate("SelectBooksToShelf", "Form", nullptr));
        cancelButton->setText(QCoreApplication::translate("SelectBooksToShelf", "CANCEL", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectBooksToShelf: public Ui_SelectBooksToShelf {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTBOOKSTOSHELF_H
