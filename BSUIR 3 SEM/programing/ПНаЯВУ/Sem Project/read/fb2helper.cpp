#include "fb2helper.h"
#include "mainwindow.h"
#include <qfile.h>
#include <qxmlstream.h>

fb2helper::fb2helper(){}
Book fb2helper::convertFb2ToHTML(QString filePath)
{

    QString book;
    QStringList content;
    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // qDebug() << "файл не открыт";
    }
    bool ok = true;
    QString special;
    QString description; // описание
    //  настройки отображения
    int fontSize = 20;
    QXmlStreamReader sr(&f);
    QString rId;
    QString rType;
    QStringList titles;
    QStringList authors;
    QString opt;
    QStringList images;
    QStringList thisToken;
    bool insideTitleInfo = false;
    QString author;
    QString title;
    while( !sr.atEnd() )
    {
        switch( sr.readNext() )
        {
        case QXmlStreamReader::NoToken:
            break;
        case QXmlStreamReader::StartDocument:
            book = "<!DOCTYPE HTML><html><body style=\"font-size:%1px; font-family:%2;\">";
            break;
        case QXmlStreamReader::EndDocument:
            book.append("</body></html>");
            break;
        case QXmlStreamReader::StartElement:
            thisToken.append( sr.name().toString() );

            if(thisToken.contains("description")) // ОПИСАНИЕ КНИГИ
            {
                if( thisToken.back() != "image" ) //пропускаем всё кроме обложки
                    break; // не выводим

            }
            if (sr.name().toString() == "title-info")
            {
                insideTitleInfo = true;
            }
            else if (insideTitleInfo && sr.name().toString() == "author")
            {
                while (sr.readNextStartElement())
                {
                    if (sr.name().toString() == "first-name" || sr.name().toString() == "last-name")
                    {
                        author += sr.readElementText() + " ";
                    }
                }
            }
            else if (insideTitleInfo && sr.name().toString() == "book-title")
            {
                title = sr.readElementText();
            }
            if(sr.name().toString() == "title")
            {
                content.append(""); // добавляем пункт содержания
                break;
            }

            if( sr.name().toString() == "body" )
                if( !sr.attributes().isEmpty()
                    && sr.attributes().first().value().toString() == "notes")
                    special = "notes";  // режим примечаний

            if(special == "notes")
            {
                if( sr.name().toString() == "section" )
                {
                    if( sr.attributes().count() > 0 )
                    {
                        rId = sr.attributes().at(0).value().toString(); // ссылка на текст
                        rType = "";
                    }
                }
            }

            opt = " align=\"justify\"";
            if(thisToken.contains("title") )
            {
                opt = " align=\"center\" style=\"font-size:" +QString::number(int(fontSize * 1.5)) + "px\" ";
                if(special == "notes")
                {
                    opt += (" id=\"" + rId + "\"");
                }
            }
            if(thisToken.contains("subtitle") )
            {
                opt = " align=\"center\" style=\"font-size:" +QString::number(int(fontSize * 1.2)) + "px\" ";
            }
            if(thisToken.contains("annotation") )
            {
                opt = " align=\"left\" ";
            }

            if(sr.name().toString() == "p"
                || sr.name().toString() == "subtitle")
            {
                book.append("<p"+opt +" >");
                break;
            }




            if( sr.name().toString() == "table" )
            {
                QString text;
                for(int i = 0; i < sr.attributes().count(); i++)
                {
                    if(sr.attributes().at(i).name() == QString("id"))
                        //         qDebug() << sr.attributes().at(i).value().toString();
                        if(sr.attributes().at(i).name() == QString("style"))
                            text.append( "style=\"" +sr.attributes().at(i).value().toString()+ ";\"" );
                }
                book.append("<table border=1 align=\"center\" style=\"border:solid;\" " + text + ">");
                break;
            }
            if( sr.name().toString() == "tr" )
            {
                QString text;
                if(!thisToken.contains("table"))
                    //      qDebug() << "ошибка в таблице";
                    for(int i = 0; i < sr.attributes().count(); i++)
                    {
                        if(sr.attributes().at(i).name() == QString("aling"))
                            text.append( "aling=\"" +sr.attributes().at(i).value().toString()+ "\"" );
                        else
                            qDebug() << "<tr>" << sr.attributes().at(i).name() << sr.attributes().at(i).value().toString();
                    }
                book.append("<tr " + text + ">");
                break;
            }            //
            if( sr.name().toString() == "td"
                || sr.name().toString() == "th" )
            {

                QString text;
                for(int i = 0; i < sr.attributes().count(); i++)
                {
                    if(sr.attributes().at(i).name() == QString("aling"))
                        text.append( "aling=\"" +sr.attributes().at(i).value().toString()+ "\" " );
                    else if(sr.attributes().at(i).name() == QString("valing"))
                        text.append( "valing=\"" +sr.attributes().at(i).value().toString()+ "\" " );
                    else if(sr.attributes().at(i).name() == QString("colspan"))
                        text.append( "colspan=" +sr.attributes().at(i).value().toString()+ " " );
                    else if(sr.attributes().at(i).name() == QString("rowspan"))
                        text.append( "rowspan=" +sr.attributes().at(i).value().toString()+ " " );
                    else
                        qDebug() << "<td th>" << sr.attributes().at(i).name() << sr.attributes().at(i).value().toString();
                }
                book.append( "<"+sr.name().toString()+ " " + text +">" );
                break;
            }
            if( sr.name().toString() == "empty-line" )
            {
                book.append("<br/>");
                break;
            }
            if(sr.name().toString() == "strong"
                || sr.name().toString() == "sup"
                || sr.name().toString() == "sub"
                || sr.name().toString() == "code"
                || sr.name().toString() == "cite")
            {
                book.append( "<" + sr.name().toString() + ">");
                break;
            }
            if(sr.name().toString() == "emphasis")
            {
                book.append( "<i>" );
                break;
            }
            if( sr.name().toString() == "v" )
            {
                book.append("<p align=\"left\" style=\"margin-left:25px;\">");
                break;
            }
            if(sr.name().toString() == "strikethrough")
            {
                book.append( "<strike>" );
                break;
            }

            if( sr.name().toString() == "a" )
            {
                rId = "";
                for(int i = 0; i < sr.attributes().count(); i++)
                {
                    if(sr.attributes().at(i).name() == QString("type") )
                    {
                        //rType = sr.attributes().at(i).value().toString();
                    }
                    if(sr.attributes().at(i).name() == QString("href"))
                    {
                        rId = sr.attributes().at(i).value().toString();
                    }
                }
                book.append("<a href=\"" + rId + "\"> ");
            }

            if(sr.name().toString() == "poem"
                || sr.name().toString() == "stanza"
                || sr.name().toString() == "epigraph")
            {
                break;
            }

            if(sr.name().toString() == "text-author" ) // автор текстта
            {
                book.append( "<p align=\"justify\" style=\"margin-left:45px;\">" );
                break;
            }
            if(sr.name().toString() == "date" ) // автор текстта
            {
                book.append( "<p align=\"justify\" style=\"margin-left:45px;\">" );
                break;
            }

            if( sr.name().toString() == "image" ) // расположение рисунков
            {
                if(sr.attributes().count() > 0)
                    book.append("<p align=\"center\">"+sr.attributes().at(0).value().toString() + "#" + "</p>");
            }
            if(sr.name() == QString("binary")) // хранилище рисунков
            {
                if(sr.attributes().at(0).name() == QString("id"))
                {
                    rId = sr.attributes().at(0).value().toString();
                    rType = sr.attributes().at(1).value().toString();
                }
                if(sr.attributes().at(1).name() == QString("id"))
                {
                    rId = sr.attributes().at(1).value().toString();
                    rType = sr.attributes().at(0).value().toString();
                }
            }
            break;
        case QXmlStreamReader::EndElement:
            if( thisToken.last() == sr.name().toString() )
            {
                thisToken.removeLast();
            }
            else
                //    qDebug() << "error token";

                if(thisToken.contains("description")) // ОПИСАНИЕ КНИГИ
                {
                    break; // не выводим
                }
            if (sr.name().toString() == "title-info")
            {
                insideTitleInfo = true;
            }
            else if (insideTitleInfo && sr.name().toString() == "author")
            {
                while (sr.readNextStartElement())
                {
                    if (sr.name().toString() == "first-name" || sr.name().toString() == "last-name")
                    {
                        author += sr.readElementText() + " ";
                    }
                }
            }
            else if (insideTitleInfo && sr.name().toString() == "book-title")
            {
                title = sr.readElementText();
            }
            if( sr.name().toString() == "p"
                || sr.name().toString() == "subtitle"
                || sr.name().toString() == "v"
                || sr.name().toString() == "date"
                || sr.name().toString() == "text-author")
            {
                book.append("</p>");
                break;
            }
            if(sr.name().toString() == "text-autor")
                authors.append(sr.name().toString());
            if(sr.name().toString() == "td"
                || sr.name().toString() == "th"
                || sr.name().toString() == "tr"
                || sr.name().toString() == "table"
                || sr.name().toString() == "sup"
                || sr.name().toString() == "sub"
                || sr.name().toString() == "strong"
                || sr.name().toString() == "code"
                || sr.name().toString() == "cite")
            {
                book.append( "</"+sr.name().toString()+">" );
                break;
            }

            if( sr.name().toString() == "a" )
            {
                rId.remove("#");
                book.append( "</a><span id=\"" + rId + "___" + "\"></span>" );
                //    qDebug() << "id" << rId + "___";
                break;
            }

            if(sr.name().toString() == "emphasis")
            {
                book.append( "</i>" );
                break;
            }
            if(sr.name().toString() == "strikethrough")
            {
                book.append( "</strike>" );
                break;
            }




            if(sr.name().toString() == "stanza") // конец строфы
            {
                break;
            }
            if(sr.name().toString() == "epigraph"
                || sr.name().toString() == "poem")
            {
                break;
            }

            if(special == "notes") // режим извлечения примечаний
            {
                if( sr.name().toString() == "body" )
                {
                    special = "";
                }
                if( sr.name().toString() == "section" )
                {
                    book.insert(book.lastIndexOf("<"), "<a href=\"#" + rId + "___" + "\"> назад</a>");
                }
            }
            break;
        case QXmlStreamReader::Characters:
            if( sr.text().toString() == "" )
            {
                break;
            }
            if( sr.text().toString() == "\n" )
            {
                break;
            }

            if(thisToken.contains("description")) // ОПИСАНИЕ КНИГИ
            {
                description.append(sr.text().toString() + " "); // не выводим
                break;
            }
            if (sr.name().toString() == "title-info")
            {
                insideTitleInfo = true;
            }
            else if (insideTitleInfo && sr.name().toString() == "author")
            {
                while (sr.readNextStartElement())
                {
                    if (sr.name().toString() == "first-name" || sr.name().toString() == "last-name")
                    {
                        author += sr.readElementText() + " ";
                    }
                }
            }
            else if (insideTitleInfo && sr.name().toString() == "book-title")
            {
                title = sr.readElementText();
            }
            if(thisToken.contains( "binary" ) ) // для рисунков
            {

                QString image = "<img src=\"data:"
                                + rType +";base64,"
                                + sr.text().toString()
                                + "\"/>";
                images.append(image);
                book.replace("#"+rId +"#", image);
                rId = "";
                rType = "";

                break;
            }
            if(thisToken.contains("div"))
            {
                break;
            }
            if(thisToken.back() == "FictionBook")
            {
                break;
            }

            if( thisToken.contains("title") ) // формируем содержание
            {
                titles.append(sr.text().toString());
                content.back() += " " + sr.text().toString();//content->back()=="" ? "" : " " +
                //  qDebug() << "title" << sr.text().toString();
            }

            if(special == "notes" && !thisToken.contains("title") )
            {
                rType += " ";
                rType += sr.text().toString();
            }

            if(thisToken.back() == "p"
                || thisToken.back() == "subtitle"
                || thisToken.back() == "v"
                || thisToken.back() == "emphasis"
                || thisToken.back() == "strong"
                || thisToken.back() == "strikethrough"
                || thisToken.back() == "sup"
                || thisToken.back() == "sub"
                || thisToken.back() == "td"
                || thisToken.back() == "th"
                || thisToken.back() == "code"
                || thisToken.back() == "cite"
                || thisToken.back() == "text-author"  // ??
                || thisToken.back() == "date"
                )
            {
                book.append( sr.text().toString() );
                break;
            }

            if(thisToken.back() == "section")
            {
                break;
            }
            if(thisToken.back() == "body")
            {
                break;
            }
            if(thisToken.back() == "table"
                || thisToken.back() == "tr"
                || thisToken.back() == "title"
                || thisToken.back() == "poem"
                || thisToken.back() == "stanza")
            {
                break;
            }
            if(thisToken.back() == "annotation")
            {
                qDebug() << "annotation" << sr.text().toString();
                break;
            }




            if(thisToken.back() == "a")
            {
                book.append( sr.text().toString() );
                break;
            }
            //все прочие тэги
            if( !sr.text().toString().isEmpty() )
            {
                qDebug() << thisToken.back() <<  "исключение" ;
                book.append("<span> " + sr.text().toString() + "</span>");
            }
            break;
        }
    }
    f.close();
    Book book_obj= parseFictionBook(filePath);


    if(!images.isEmpty())
    {
        book_obj.setImage(images[0]);
    }
    book_obj.setHtml(book);
    return book_obj;
}
Book fb2helper::parseFictionBook(QString filePath) {
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Failed to open the file.";

    }
    else{
        QXmlStreamReader xmlReader(&file);

        QString firstName, lastName, bookTitle;
        bool isFirstName=false, islastName=false, isTitle=false;
        while (!xmlReader.atEnd() && !xmlReader.hasError()) {
            xmlReader.readNext();
            if (xmlReader.isStartElement()) {
                if (!isFirstName && xmlReader.name() == QString("first-name")) {
                    isFirstName=true;
                    firstName = xmlReader.readElementText();
                } else if (!islastName && xmlReader.name() == QString("last-name")) {
                    islastName=true;
                    lastName = xmlReader.readElementText();
                } else if (!isTitle && xmlReader.name() == QString("book-title")) {
                    isTitle=true;
                    bookTitle = xmlReader.readElementText();

                }

            }

        }


        if (xmlReader.hasError()) {
            qDebug() << "XML parsing error: " << xmlReader.errorString();
        } else {
            qDebug() << "First Name: " << firstName;
            qDebug() << "Last Name: " << lastName;
            qDebug() << "Book Title: " << bookTitle;
        }

        file.close();
        Book book;
        book.setAuthor(firstName+" "+lastName);
        book.setTitle(bookTitle);
        return book;
    }
}
