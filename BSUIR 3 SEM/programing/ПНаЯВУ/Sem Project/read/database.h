#ifndef DATABASE_H
#define DATABASE_H
#include "Book.h"

#include <QCoreApplication>
#include <QtSql>
#include "shelf.h"

class database
{

private:
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");


public:
    database();
    ~database();
    void addBook(Book book);
    std::vector<Book>  getAllBooks();
    void deleteBook(int id);
    void addShelf(QString shelfName);
    std::vector<shelf> getAllShelves();
    void addBookOnShelf(int shelfId, int bookId);
    void deleteBookFromShelf(int shelfId, int bookId);
    void deleteShelf(int shelfId);
    std::vector<Book> getBooksFromShelf(int shelfId);
};

#endif // DATABASE_H
