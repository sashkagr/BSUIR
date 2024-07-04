#include "book.h"

void Book::setTitle(QString title)
{
     this->title=title;
}
QString Book::getTitle()
{
      return title;
}
void Book::setAuthor(QString author)
{
     this->author=author;
}
QString Book::getAuthor()
{
      return author;
}
void Book::setImage(QString image)
{
     this->image=image;
}
QString Book::getImage()
{
      return image;
}
void Book::setHtml(QString html)
{
    this->html=html;
}
QString Book::getHtml()
{
    return html;
}

Book::Book(int id): BaseEntity(id){}

Book::Book():BaseEntity(){}
