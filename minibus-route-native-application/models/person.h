#ifndef PERSON_H
#define PERSON_H

#include <QObject>
#include <QVector>
#include <QSqlRecord>
#include "global.h"

class Person : public QObject
{
    Q_OBJECT
public:
    explicit Person(QObject *parent = nullptr, Person *person = nullptr);

    PersonType personType() const;

    QVector<QSqlRecord> personData() const;
    void setPersonData(const QVector<QSqlRecord> &personData);

signals:

private:

    PersonType person_type_;
    QVector<QSqlRecord> person_data_;

    void setPersonType(const PersonType &personType);

};

#endif // PERSON_H
