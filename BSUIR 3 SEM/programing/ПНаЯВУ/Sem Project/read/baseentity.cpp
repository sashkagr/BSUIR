#include "baseentity.h"

BaseEntity::BaseEntity(int id)
{
    this->id=id;
}

BaseEntity::BaseEntity(){}

int BaseEntity::getId() const
{
    return id;
}

void BaseEntity::setId(int newId)
{
    id = newId;
}

