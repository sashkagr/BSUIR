#ifndef FB2HELPER_H
#define FB2HELPER_H
#include <QString>
#include <QStringList>
#include "book.h"

class fb2helper
{
private:

public:
    fb2helper();
    static Book convertFb2ToHTML(QString filePath);
    static Book parseFictionBook(QString filePath);
};

#endif // FB2HELPER_H
