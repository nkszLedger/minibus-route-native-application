#include "person.h"

Person::Person(QObject *parent, Person *person) : QObject(parent)
{
    this->setPersonType(person->personType());
    this->setPersonData(person->personData());
}

PersonType Person::personType() const
{
    return person_type_;
}

QVector<QSqlRecord> Person::personData() const
{
    return person_data_;
}

void Person::setPersonData(const QVector<QSqlRecord> &personData)
{
    person_data_ = personData;
}

void Person::setPersonType(const PersonType &personType)
{
    person_type_ = personType;
}
