#ifndef SHELVESLIST_H
#define SHELVESLIST_H

#include <QDialog>
#include <vector>
#include "shelf.h"
#include "database.h"
#include <QString>
#include <vector>

namespace Ui {
class ShelvesList;
}

class ShelvesList : public QDialog
{
    Q_OBJECT

public:
    explicit ShelvesList(QWidget *parentProxy, QWidget *parent = nullptr);
    ~ShelvesList();
    void showAllShalves();

public slots:
    void onListItemClicked(int id, QString name);

private slots:
    void on_exitButton_clicked();

    void on_addShelfBtn_clicked();

private:
    Ui::ShelvesList *ui;
    QWidget *parentWindow;
    std::vector<shelf> shelves;
    database *db;

};

#endif // SHELVESLIST_H
