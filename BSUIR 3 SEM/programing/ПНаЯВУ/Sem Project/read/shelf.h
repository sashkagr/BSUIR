#ifndef SHELF_H
#define SHELF_H
#include <QString>
#include "baseentity.h"


class shelf: public BaseEntity
{
private:
    QString naming;
public:
    shelf();
    void setNaming(QString naming);
    QString getNaming();
};

#endif // SHELF_H
