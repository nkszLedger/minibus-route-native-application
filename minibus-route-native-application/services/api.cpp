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
    /* init database */
    Database db;

    /* Declare & sanitize db fields */
    QString table = "fingerprints";

    /* populate fields */
    QDateTime dt = QDateTime::currentDateTime();
    QString today = dt.toString("yyyy-MM-dd HH:mm:ss");
    qDebug() << "api::postCapturedFingerprint() - Date Today: " << today;

    if( db.connOpen() )
    {
        if( db.insertTemplate(table, "fingerprint", member_id, today, today, image) )
        {
           qDebug() << "api::postCapturedFingerprint() - Fingerprint POST successful";
           return true;
        }
        else
        {
            qDebug() << "api::postCapturedFingerprint() - Fingerprint POST failed";
            return false;
        }
    }
    else
    {
        qDebug() << "api::postCapturedFingerprint() - DB Connection failed";
    }

    return false;
}

bool api::postCapturedPortrait(QString member_id, QByteArray image)
{
    /* init database */
    Database db;

    /* Declare & sanitize db fields */
    QString table = "portraits";

    /* populate fields */
    QDateTime dt = QDateTime::currentDateTime();
    QString today = dt.toString("yyyy-MM-dd HH:mm:ss");
    qDebug() << "api::postCapturedPortrait() - Date Today: " << today;

    if( db.connOpen() )
    {
        if( db.insertTemplate(table, "portrait", member_id, today, today, image) )
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
