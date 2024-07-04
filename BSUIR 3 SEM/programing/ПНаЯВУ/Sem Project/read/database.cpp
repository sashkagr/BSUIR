#include "Book.h"
#include "database.h"

#include <vector>

database::database()
{
db.setDatabaseName("C:/Users/sashk/OneDrive/Pulpit/bonstic/kursach/read/qtreader.db");
    if (db.open()) {
        qDebug() << "Успешное подключение к серверу MySQL!";
        QSqlQuery query;
        query.exec("PRAGMA foreign_keys = ON;");
    }
}
database::~database()
{
    if (db.open()) {
        qDebug() << "Успешное отключение к серверу MySQL!";
        db.close();
    } else {
        qDebug() << "сервер SQLite не запущен: " << db.lastError().text();
    }
}

void database::addBook(Book book)
{
    QSqlQuery query;
    QString insertQuery = "INSERT INTO Book (title, author, img, content) VALUES (?, ?, ?, ?)";
    query.prepare(insertQuery);

    query.bindValue(0, book.getTitle());
    query.bindValue(1, book.getAuthor());
    query.bindValue(2, book.getImage());
    query.bindValue(3, book.getHtml());

    if (query.exec()) {
        qDebug() << "Insert successful.";
    } else {
        qDebug() << "Insert failed: " << query.lastError().text();
    }
}

std::vector<Book> database::getAllBooks()
{
    std::vector<Book> books;

    QString sqlQuery = "SELECT * FROM Book;";
    QSqlQuery query;
    if (query.exec(sqlQuery)) {

        while (query.next()) {
            Book book;

            int id = query.value(0).toInt();
            QString title = query.value(1).toString();
            QString author = query.value(2).toString();
             QString image = query.value(3).toString();
              QString html = query.value(4).toString();

             book.setId(id);
             book.setTitle(title);
             book.setAuthor(author);
             book.setImage(image);
             book.setHtml(html);
             books.push_back(book);
        }
    } else {
        qDebug() << "Query failed: " << query.lastError().text();
    }
    return books;
}

void database::deleteBook(int id)
{
    QSqlQuery query;
    QString deleteBookFromAllShelvesQuery = "DELETE FROM BooksOnShelfes WHERE book_id=?";
    QString deleteBookQuery = "DELETE from Book where id=?";

    if (db.transaction())
    {
        query.prepare(deleteBookFromAllShelvesQuery);
        query.bindValue(0,id);
        if (query.exec()) {
             qDebug() << "Book deleted from all shelves.";
        } else {
             qDebug() << "Delete book from all shelves failed: " << query.lastError().text();
        }

        query.prepare(deleteBookQuery);
        query.bindValue(0,id);
        if (query.exec()) {
             qDebug() << "Book deleted.";
        } else {
             qDebug() << "Delete failed: " << query.lastError().text();
        }

        if (db.commit())
        {
             qDebug() << "Transation completed.";
        }
        else
        {
             db.rollback();
        }
    }
    else
    {
        qDebug() << "Unable to start transaction";
    }
}

void database::addShelf(QString shelfName)
{
    QSqlQuery query;
    QString insertQuery = "INSERT INTO Shelf (naming) VALUES (?);";
    query.prepare(insertQuery);

    query.bindValue(0, shelfName);

    if (query.exec()) {
        qDebug() << "Insert successful.";
    } else {
        qDebug() << "Insert failed: " << query.lastError().text();
    }
}

std::vector<shelf> database::getAllShelves()
{
    std::vector<shelf> shelves;

    QString sqlQuery = "SELECT * FROM Shelf;";
    QSqlQuery query;
    if (query.exec(sqlQuery)) {

        while (query.next()) {
             shelf shelf;

             int id = query.value(0).toInt();
             QString naming = query.value(1).toString();

             shelf.setId(id);
             shelf.setNaming(naming);
             shelves.push_back(shelf);
        }
    } else {
        qDebug() << "Query failed: " << query.lastError().text();
    }
    return shelves;
}

void database::addBookOnShelf(int shelfId, int bookId)
{
    QSqlQuery query;
    QString insertQuery = "INSERT INTO BooksOnShelfes (shelf_id, book_id) VALUES (?,?)";
    query.prepare(insertQuery);

    query.bindValue(0, shelfId);
    query.bindValue(1, bookId);

    if (query.exec()) {
        qDebug() << "Insert successful.";
    } else {
        qDebug() << "Insert failed: " << query.lastError().text();
    }
}

void database::deleteBookFromShelf(int shelfId, int bookId)
{
    QSqlQuery query;
    QString insertQuery = "DELETE FROM BooksOnShelfes WHERE book_id = ? AND shelf_id = ?";
    query.prepare(insertQuery);

    query.bindValue(0, bookId);
    query.bindValue(1, shelfId);

    if (query.exec()) {
        qDebug() << "Insert successful.";
    } else {
        qDebug() << "Insert failed: " << query.lastError().text();
    }
}

void database::deleteShelf(int shelfId)
{
    QSqlQuery query;
    QString deleteAllBooksFromShelfQuery = "DELETE FROM BooksOnShelfes WHERE shelf_id=?";
    QString deleteShelfQuery = "DELETE FROM Shelf WHERE id=?";

        query.prepare(deleteAllBooksFromShelfQuery);
        query.bindValue(0, shelfId);
        if (query.exec()) {
             qDebug() << "All books deleted from shelf.";
        } else {
             qDebug() << "Delete all books from shelf failed: " << query.lastError().text();
        }

        query.prepare(deleteShelfQuery);
        query.bindValue(0, shelfId);
        if (query.exec()) {
             qDebug() << "Shelf deleted.";
        } else {
             qDebug() << "Delete failed: " << query.lastError().text();
        }
}

std::vector<Book> database::getBooksFromShelf(int shelfId)
{
    std::vector<Book> books;

    QString sqlQuery = "SELECT Book.id, Book.title, Book.author, Book.img, Book.content FROM Book INNER JOIN BooksOnShelfes ON BooksOnShelfes.book_id = Book.id INNER JOIN Shelf ON Shelf.id = BooksOnShelfes.shelf_id WHERE Shelf.id = ?";
    QSqlQuery query;
    query.prepare(sqlQuery);
    query.bindValue(0, shelfId);
    if (query.exec()) {

        while (query.next()) {
             Book book;

             int id = query.value(0).toInt();
             QString title = query.value(1).toString();
             QString author = query.value(2).toString();
             QString image = query.value(3).toString();
             QString html = query.value(4).toString();

             book.setId(id);
             book.setTitle(title);
             book.setAuthor(author);
             book.setImage(image);
             book.setHtml(html);
             books.push_back(book);
        }
    } else {
        qDebug() << "Query failed: " << query.lastError().text();
    }
    return books;
}
