#include "api.h"

api *api::api_instance_ = 0;

api::api(QObject *parent) : QObject(parent)
{
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
    mode_ = AUTH;

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

void api::isMemberRegistered(QString id)
{
    mode_ = GET_MEMBER_DETAILS;

    /* setup the webservice LOGIN url */
    QUrl serviceUrl = QUrl( base_url_ + "/api/members/" + id);

    /* set format */
    QString sAuth = "Bearer " + auth_token_;
    QByteArray bAuth = QByteArray::fromStdString(sAuth.toStdString());

    QNetworkRequest networkRequest(serviceUrl);

    networkRequest.setRawHeader( "Authorization", bAuth );
    networkRequest.setRawHeader( "Accept", "application/json");
    networkRequest.setHeader( QNetworkRequest::ContentTypeHeader, \
                              "application/x-www-form-urlencoded");

    /* post details lookup request */
    manager_->get( networkRequest );
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
        QJsonObject jsonSuccess;

        if( statusCode == 200 )
        {
            switch( mode_ )
            {

                case AUTH:
                    jsonSuccess = jsonObject[ "success" ].toObject();
                    auth_token_ = jsonSuccess.value("token").toString();

                    qDebug() << auth_token_ << endl;
                    emit auth_successful();

                    /* request details after */
                    //QByteArray byte;
                    //getMemberFingerprints("0502030603081", byte);
                break;

                case GET_MEMBER_DETAILS:
                    qDebug() << "api::replyFinished() - Response" << endl;

                    jsonSuccess = jsonObject[ "success" ].toObject();
                    qDebug() << jsonObject << endl;
                    QString created_at = jsonSuccess.value("created_at").toString();
                    qDebug() << created_at << endl;
                    emit member_details_found(jsonObject);
                break;
            }
        }
        else
        {
            switch( mode_ )
            {
                case AUTH: emit auth_failed();
                break;
                case GET_MEMBER_DETAILS: emit member_details_not_found();
                break;
            }
            qDebug() << "api::replyFinished() - Unexpected Error occured with code: " << statusCode;
        }
        qDebug() << "api::replyFinished() - Finished";
   }
}
