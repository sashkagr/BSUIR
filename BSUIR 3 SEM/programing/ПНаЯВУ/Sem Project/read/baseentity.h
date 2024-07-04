#ifndef BASEENTITY_H
#define BASEENTITY_H


class BaseEntity
{
private:
    int id;
public:
    BaseEntity(int id);
    BaseEntity();
    int getId() const;
    void setId(int newId);
};

#endif // BASEENTITY_H
