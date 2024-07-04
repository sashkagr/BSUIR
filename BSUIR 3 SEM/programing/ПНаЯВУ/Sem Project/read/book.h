#ifndef BOOK_H
#define BOOK_H
#include "baseentity.h"
#include <qstring.h>

class Book: public BaseEntity
{
private:

    QString title;
    QString author;
    QString image;
    QString html;


public:
    void setTitle(QString title);
    QString getTitle();
    void setAuthor(QString author);
    QString getAuthor();
    void setImage(QString image);
    QString getImage();
    void setHtml(QString html);
    QString getHtml();
    Book(int id);
    Book();
};

#endif // BOOK_H
