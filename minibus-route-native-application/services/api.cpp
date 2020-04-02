#include "api.h"

api::api(QObject *parent) : QObject(parent)
{

}

bool api::authenticateUser()
{
    return true;
}

bool api::isMemberRegistered()
{
    return true;
}

bool api::postCapturedFingerprint(QString member_id, QByteArray image)
{
    return true;
}

bool api::postCapturedPortrait(QString member_id, QByteArray image)
{
    /* init database */
    Database db;
    db.setDriver("QPSQL");
    db.setPort(5432);
    db.setHost("146.64.35.16");
    db.setDbname("minibus_route_app_db");
    db.setUsername("postgres");
    db.setPassword("BioAcqApp2016");

    /* Declare & sanitize db fields */
    QString table = "potraits";
    QVector<QString> column_list;
    column_list.clear();
    QVector<QString> value_list ;
    value_list.clear();

    /* populate fields */
    QDateTime dt;
    QString today = dt.toString();
    qDebug() << "api::postCapturedPortrait() - Date Today: " << today;

    column_list = {"member_id", "portrait", "created_at", "updated_at"};
    value_list = {member_id, image, today, today };

    if( db.connOpen() )
    {
        if( db.insert(table, column_list, value_list ) )
        {
           qDebug() << "api::postCapturedPortrait() - Potrait POST successful";
           return true;
        }
        else
        {
            qDebug() << "api::postCapturedPortrait() - Potrait POST failed";
            return false;
        }
    }
    else
    {
        qDebug() << "api::postCapturedPortrait() - DB Connection failed";
    }

    return false;
}

void api::getCapturedFingerprint(QByteArray &image)
{

}

void api::getCapturedPortrait(QByteArray &image)
{

}
