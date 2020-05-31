#include "api.h"

api *api::api_instance_ = 0;

api::api(QObject *parent) : QObject(parent)
{
    is_authenticating_ = false;

    manager_ = new QNetworkAccessManager(this);

    connect(manager_, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    initConnection("127.0.0.1", 8000);
}

void api::initConnection(QString address, int port)
{
    /* set url */
    base_url_ = "http://" + address + ":" + QString::number(port);

    /* connect to host via http on port */
    urlookup_ = new QUrl( base_url_ );

    /* connect to share */
    manager_->connectToHost(address, port);
}

void api::authenticateUser(QString username, QString password)
{
    is_authenticating_ = true;

    /* setup the webservice LOGIN url */
    QUrl serviceUrl = QUrl( base_url_ + "/api/login?" );
    QByteArray postData;

    QUrlQuery query;
    query.addQueryItem( "email", username );
    query.addQueryItem( "password", password );

    /* actual data to be posted */
    postData = query.toString(QUrl::FullyEncoded).toUtf8();

    /* print url */
    qDebug() << "api::authenticateUser() - Post Data ";
    qDebug() << postData;

    /* set format */
    QNetworkRequest networkRequest(serviceUrl);
    networkRequest.setHeader( QNetworkRequest::ContentTypeHeader, \
                              "application/x-www-form-urlencoded");

    /* post login request */
    manager_->post( networkRequest, postData );

    /* set auth token */
}

bool api::isMemberRegistered(QString id_type, QString id, QVector<QSqlRecord> &result)
{
    return true;

    /* init database */
    Database db;

    /* Declare & sanitize db fields */
    QString table = "members";
    QVector<QString> select_columns = { "id", "id_number", "license_number", "name", "surname" };
    QVector<QString> column_list;
    QVector<QString> value_list;

    if( id_type == "license_number" )
    {
        column_list = { "license_number" };
        value_list = { id };
    }
    else
    {
        column_list = { "id_number" };
        value_list = { id };
    }

    /* populate fields */
    QDateTime dt = QDateTime::currentDateTime();
    QString today = dt.toString("yyyy-MM-dd HH:mm:ss");
    qDebug() << "api::isMemberRegistered() - Date Today: " << today;

    if( db.connOpen() )
    {
        if( db.select(table, select_columns, column_list, value_list, result) )
        {
           qDebug() << "api::isMemberRegistered() - Member verification successful";
           qDebug() << "api::isMemberRegistered() - Result: " << result;
           db.connClosed();
           return true;
        }
        else
        {
            qDebug() << "api::isMemberRegistered() - Member verification failed";
        }
    }
    else
    {
        qDebug() << "api::isMemberRegistered() - DB Connection failed";
    }
    return false;
}

bool api::postCapturedFingerprint(QString member_id, QByteArray image, bool is_an_update)
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
        if( is_an_update )
        {
            if( db.updateTemplate( table, "fingerprint", member_id, today, image ))
            {
                qDebug() << "api::postCapturedFingerprint() - Fingerprint UPDATE successful";
                return true;
            }
            else
            {
                qDebug() << "api::postCapturedFingerprint() - Fingerprint UPDATE failed";
            }
        }
        else
        {
            if( db.insertTemplate(table, "fingerprint", member_id, today, today, image) )
            {
               qDebug() << "api::postCapturedFingerprint() - Fingerprint POST successful";
               return true;
            }
            else
            {
                qDebug() << "api::postCapturedFingerprint() - Fingerprint POST failed";
            }
        }
    }

    db.connClosed();
    return false;
}

bool api::postCapturedPortrait(QString member_id, QByteArray image, bool is_an_update)
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
        if( is_an_update )
        {
            if( db.updateTemplate( table, "portrait", member_id, today, image ))
            {
                qDebug() << "api::postCapturedPortrait() - Fingerprint UPDATE successful";
                return true;
            }
            else
            {
                qDebug() << "api::postCapturedPortrait() - Potrait UPDATE failed";
            }
        }
        else
        {
            if( db.insertTemplate(table, "portrait", member_id, today, today, image) )
            {
               qDebug() << "api::postCapturedPortrait() - Potrait POST successful";
               return true;
            }
            else
            {
                qDebug() << "api::postCapturedPortrait() - Potrait POST failed";
            }
        }
    }
    else
    {
        qDebug() << "api::postCapturedPortrait() - DB Connection failed";
    }

    db.connClosed();
    return false;
}

bool api::getCapturedFingerprint(QString member_id, QByteArray &image)
{
    /* init database */
    Database db;

    /* Declare & sanitize db fields */
    QString table = "fingerprints";
    QVector<QString> select_columns = { "fingerprint" };
    QVector<QString> column_list = {"member_id"};
    QVector<QString> value_list = { member_id };
    QVector<QSqlRecord> result;

    if( db.connOpen() )
    {
        if( db.select(table, select_columns, column_list, value_list, result) )
        {
           qDebug() << "api::getCapturedFingerprint() - Member fingerprint retrieval successful";
           if( !result.isEmpty() )
           {
               image = QByteArray::fromBase64(result.at(0).field( "fingerprint" ).value().toByteArray());
           }
           db.connClosed();
           return true;
        }
        else
        {
            qDebug() << "api::getCapturedFingerprint() - Member fingerprint retrieval failed";
        }
    }
    else
    {
        qDebug() << "api::getCapturedFingerprint() - DB Connection failed";
    }
    return false;
}

bool api::getCapturedPortrait(QString member_id, QByteArray &image)
{
    /* init database */
    Database db;

    /* Declare & sanitize db fields */
    QString table = "portraits";
    QVector<QString> select_columns = { "portrait" };
    QVector<QString> column_list = {"member_id"};
    QVector<QString> value_list = { member_id };
    QVector<QSqlRecord> result;

    if( db.connOpen() )
    {
        if( db.select(table, select_columns, column_list, value_list, result) )
        {
           qDebug() << "api::getCapturedPortrait() - Member portrait retrieval successful";
           if( !result.isEmpty() )
           {
               image = QByteArray::fromBase64(result.at(0).field( "portrait" ).value().toByteArray());
           }

           db.connClosed();
           return true;
        }
        else
        {
            qDebug() << "api::getCapturedPortrait() - Member portrait retrieval failed";
        }
    }
    else
    {
        qDebug() << "api::getCapturedPortrait() - DB Connection failed";
    }
    return false;
}

void api::replyFinished(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString strReply = (QString)bytes;
    json_response_ = QJsonDocument::fromJson(strReply.toUtf8());

    if( !bytes.isEmpty() )
    {
        QString str = QString::fromUtf8(bytes.data(), bytes.size());
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        qDebug() << "api::replyFinished() - Status Code: " << QVariant(statusCode).toString();
        QJsonObject jsonObject = json_response_.object();

        if( statusCode == 200 )
        {
            if( is_authenticating_ )
            {
                QJsonObject jsonSuccess = jsonObject[ "success" ].toObject();
                auth_token_ = jsonSuccess.value("token").toString();

                qDebug() << auth_token_ << endl;
                emit auth_successful();

                /* request details after */
                //QByteArray byte;
                //getMemberFingerprints("0502030603081", byte);
            }
            else
            {
                qDebug() << "api::replyFinished() - Response" << endl;

                QJsonObject jsonSuccess = jsonObject[ "success" ].toObject();
                qDebug() << jsonObject << endl;
                QString created_at = jsonSuccess.value("created_at").toString();
                qDebug() << created_at << endl;

//                QStringList propertyNames;

//                foreach (const QJsonValue & value, jsonArray)
//                {
//                    QJsonObject obj = value.toObject();
//                    propertyNames.append(obj["ID"].toString());
//                    propertyNames.append(obj["email_verified_at"].toString());
//                    propertyNames.append(obj["email"].toString());
//                    propertyNames.append(obj["created_at"].toString());
//                    propertyNames.append(obj["updated_at"].toString());
//                }

//                 qDebug() << propertyNames << endl;

            }
        }
        else
        {
            if( is_authenticating_ ){ emit auth_failed(); }

            is_authenticating_ = false;
            qDebug() << "api::replyFinished() - Unexpected Error occured with code: " << statusCode;
        }



    //        foreach (const QJsonValue & value, jsonArray) {
    //            QJsonObject obj = value.toObject();
    //            propertyNames.append(obj["PropertyName"].toString());
    //            propertyKeys.append(obj["key"].toString());
    //        }

        // qDebug() << tokenArray << endl;

        qDebug() << "api::replyFinished() - Finished";

        /*
        QStringList propertyNames;
        QStringList propertyKeys;
        QString strReply = (QString)reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        QJsonArray jsonArray = jsonObject["properties"].toArray();

        foreach (const QJsonValue & value, jsonArray)
        {
            QJsonObject obj = value.toObject();
            propertyNames.append(obj["PropertyName"].toString());
            propertyKeys.append(obj["key"].toString());
        }

        if(statusCode == REDIRECT)
        {
            QUrl newUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            qDebug() << "redirected from " + reply->url().toString() + " to " + newUrl.toString();
            QNetworkRequest newRequest(newUrl);
            manager_->get(newRequest);
            return;
        }
        else if( statusCode == SUCCESSFUL )
        {

        }*/
   }

}
