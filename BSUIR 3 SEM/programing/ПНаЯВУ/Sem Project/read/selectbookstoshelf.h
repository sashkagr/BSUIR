#ifndef SELECTBOOKSTOSHELF_H
#define SELECTBOOKSTOSHELF_H
#include <QDialog>
#include <vector>
#include "book.h"
#include <QString>
#include "database.h"

namespace Ui {
class SelectBooksToShelf;
}

class SelectBooksToShelf : public QDialog
{
    Q_OBJECT

public:
    explicit SelectBooksToShelf(int shelfId, std::vector<Book> booksOnShelf, QWidget *parentProxy, QWidget *parent = nullptr);
    ~SelectBooksToShelf();

private slots:
    void on_cancelButton_clicked();



private:
    Ui::SelectBooksToShelf *ui;
    QWidget *parent;
    int shelfId;
    std::vector<Book> booksOnShelf;
    database *db;

    void displayAllAvailableBooks();
    bool checkBookNotOnShelf(int bookId);
    void onListItemClicked(int bookId);
};

#endif // SELECTBOOKSTOSHELF_H
