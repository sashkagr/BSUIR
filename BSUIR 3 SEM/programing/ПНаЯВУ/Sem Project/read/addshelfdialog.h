#ifndef ADDSHELFDIALOG_H
#define ADDSHELFDIALOG_H

#include <QWidget>
#include <QDialog>
#include "database.h"
#include <vector>
#include "shelf.h"

namespace Ui {
class AddShelfDialog;
}

class AddShelfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddShelfDialog(std::vector<shelf> shelves,QWidget *parentProxy, QWidget *parent = nullptr);
    ~AddShelfDialog();

private slots:
    void on_addShelf_clicked();

    void on_cancelButton_clicked();

private:
    Ui::AddShelfDialog *ui;
    QWidget *parentWindow;
    database* db;
    std::vector<shelf> shelves;
};

#endif // ADDSHELFDIALOG_H
