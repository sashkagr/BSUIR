#include "shelf.h"

shelf::shelf()
{

}

void shelf::setNaming(QString naming)
{
    this->naming = naming;
}

QString shelf::getNaming()
{
    return this->naming;
}
