#ifndef SHELFBOOKS_H
#define SHELFBOOKS_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include <vector>
#include "book.h"
#include "database.h"
#include "reading.h"
#include <QHBoxLayout>

namespace Ui {
class ShelfBooks;
}

class ShelfBooks : public QDialog
{
    Q_OBJECT

public:
    explicit ShelfBooks(int shelfId, QString shelfName, QWidget *parentProxy, QWidget *parent = nullptr);
    ~ShelfBooks();

private slots:
    void on_shelvesButton_clicked();

    void on_addToShelf_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    int shelfId;
    QString shelfName;
    Ui::ShelfBooks *ui;
    QWidget *parent;
    database *db;
    std::vector<Book> books;
    Reading *reader;

    void displayAllBooksForShelf();
    void displayAllBooksForShelf(QString searchTitle);

    void onListItemClicked(int id,QString html);

    bool isBookTitleMatch(QString bookTitle, QString searchTitle);
};

#endif // SHELFBOOKS_H
