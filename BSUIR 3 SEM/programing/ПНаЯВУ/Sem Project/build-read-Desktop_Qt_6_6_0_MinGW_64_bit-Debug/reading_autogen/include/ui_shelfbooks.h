/********************************************************************************
** Form generated from reading UI file 'shelfbooks.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHELFBOOKS_H
#define UI_SHELFBOOKS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShelfBooks
{
public:
    QPushButton *shelvesButton;
    QLabel *shelfNameLabel;
    QPushButton *addToShelf;
    QLabel *label_2;
    QListWidget *listWidget;
    QLineEdit *lineEdit;
    QLabel *label;

    void setupUi(QWidget *ShelfBooks)
    {
        if (ShelfBooks->objectName().isEmpty())
            ShelfBooks->setObjectName("ShelfBooks");
        ShelfBooks->resize(901, 725);
        shelvesButton = new QPushButton(ShelfBooks);
        shelvesButton->setObjectName("shelvesButton");
        shelvesButton->setGeometry(QRect(780, 10, 80, 24));
        shelfNameLabel = new QLabel(ShelfBooks);
        shelfNameLabel->setObjectName("shelfNameLabel");
        shelfNameLabel->setGeometry(QRect(190, 10, 481, 16));
        addToShelf = new QPushButton(ShelfBooks);
        addToShelf->setObjectName("addToShelf");
        addToShelf->setGeometry(QRect(20, 10, 111, 24));
        label_2 = new QLabel(ShelfBooks);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(150, 10, 41, 16));
        listWidget = new QListWidget(ShelfBooks);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(30, 60, 851, 641));
        lineEdit = new QLineEdit(ShelfBooks);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(372, 10, 391, 24));
        label = new QLabel(ShelfBooks);
        label->setObjectName("label");
        label->setGeometry(QRect(320, 10, 49, 16));

        retranslateUi(ShelfBooks);

        QMetaObject::connectSlotsByName(ShelfBooks);
    } // setupUi

    void retranslateUi(QWidget *ShelfBooks)
    {
        ShelfBooks->setWindowTitle(QCoreApplication::translate("ShelfBooks", "Form", nullptr));
        shelvesButton->setText(QCoreApplication::translate("ShelfBooks", "SHELVES", nullptr));
        shelfNameLabel->setText(QCoreApplication::translate("ShelfBooks", " label name", nullptr));
        addToShelf->setText(QCoreApplication::translate("ShelfBooks", "ADD TO SHELF", nullptr));
        label_2->setText(QCoreApplication::translate("ShelfBooks", "SHELF:", nullptr));
        label->setText(QCoreApplication::translate("ShelfBooks", "Search :", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShelfBooks: public Ui_ShelfBooks {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHELFBOOKS_H
