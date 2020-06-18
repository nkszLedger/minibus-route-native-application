#include "api.h"
#include <QFile>
#include <QHttpPart>
#include <QVariant>


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
    transmission_mode_ = AUTH;

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
    transmission_mode_ = GET_MEMBER_DETAILS;

    /* setup the webservice LOGIN url */
    QUrl serviceUrl = QUrl( base_url_ + "/api/members/" + id);
    QString url = base_url_ + "/api/members/" + id;
    qDebug() << "URL: " << url;

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

void api::isUserRegistered(QString id)
{
    transmission_mode_ = GET_USER_DETAILS;

    /* setup the webservice LOGIN url */
    QUrl serviceUrl = QUrl( base_url_ + "/api/users/" + id);

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

void api::postCapturedFingerprint(QString id, QByteArray image1,
                                   QByteArray image2,
                                   AdminMode mode,
                                   bool is_an_update)
{
    transmission_mode_ = POST_MEMBER_FINGERPRINTS;

    /* init database */
    Database db;

    /* Declare & sanitize db fields */
    QString table = "member_fingerprint";

    /* populate fields */
    QDateTime dt = QDateTime::currentDateTime();
    QString today = dt.toString("yyyy-MM-dd HH:mm:ss");
    qDebug() << "api::postCapturedFingerprint() - Date Today: " << today;


    if( db.connOpen() )
    {
        if( is_an_update )
        {
            if( db.updateTemplate( table, "fingerprint2", id, today, image1, image2 ))
            {
                qDebug() << "api::postCapturedFingerprint() - Fingerprint UPDATE successful";
                db.connClosed();
                emit member_fingerprints_post_success();
            }
            else
            {
                qDebug() << "api::postCapturedFingerprint() - Fingerprint UPDATE failed";
            }

        }
        else
        {
            if( db.insertTemplate(table, "fingerprint2", id, today, today, image1, image2) )
            {
               qDebug() << "api::postCapturedFingerprint() - Fingerprint POST successful";
               db.connClosed();
               emit member_fingerprints_post_success();
            }
            else
            {
                qDebug() << "api::postCapturedFingerprint() - Fingerprint POST failed";
            }
        }
    }

    db.connClosed();
    emit member_fingerprints_post_failure();
}

void api::linkReply()
{
    qDebug() << "api::postCapturedFingerprint() - 10: ";
    connect(reply_, SIGNAL(finished()),
                  this, SLOT(multiPostReplyFinished()));

    qDebug() << "api::postCapturedFingerprint() - 11: ";

    connect(reply_, SIGNAL(uploadProgress(qint64, qint64)),
          this, SLOT  (uploadProgress(qint64, qint64)));

    qDebug() << "api::postCapturedFingerprint() - 12: ";
}

void api::postCapturedPortrait(QString id, QByteArray image,
                               AdminMode mode, bool is_an_update)
{
    /* populate fields */
    QDateTime dt = QDateTime::currentDateTime();
    QString today = dt.toString("yyyy-MM-dd HH:mm:ss");
    qDebug() << "api::postCapturedFingerprint() - Date Today: " << today;

    transmission_mode_ = POST_MEMBER_PORTRAIT;

    /* init database */
    Database db;

    /* Declare & sanitize db fields */
    QString table = "member_portrait";
    if( db.connOpen() )
    {
        if( is_an_update )
        {
            if( db.updateTemplate( table, "portrait", id, today, image ))
            {
                qDebug() << "api::postCapturedPortrait() - Portrait UPDATE successful";
                db.connClosed();
                emit member_portrait_post_success();
            }
            else
            {
                qDebug() << "api::postCapturedPortrait() - Potrait UPDATE failed";
            }
        }
        else
        {
            if( db.insertTemplate(table, "portrait", id, today, today, image) )
            {
               qDebug() << "api::postCapturedPortrait() - Potrait POST successful";
               db.connClosed();
               emit member_portrait_post_success();
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
    emit member_portrait_post_failure();
}

void api::getCapturedFingerprint(QString id, AdminMode mode)
{
    transmission_mode_ = GET_MEMBER_FINGERPRINTS;

    /* setup the webservice LOGIN url */
    QUrl serviceUrl;
    if( mode == ADMINISTER_MEMBER )
        serviceUrl = QUrl( base_url_ + "/api/membersfingerprint/" + id);
    else
        serviceUrl = QUrl( base_url_ + "/api/usersfingerprint/" + id);

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

void api::getCapturedFingerprintFromDB(QString member_id,
                                       QByteArray &image1,
                                       QByteArray &image2)
{
    /* init database */
    Database db;

    /* Declare & sanitize db fields */
    QString table = "member_fingerprint";
    QVector<QString> select_columns = { "fingerprint_left_thumb",
                                        "fingerprint_right_thumb"
                                      };
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
               qDebug() << "api::getCapturedFingerprint() - Data found";
               image1 = QByteArray::fromBase64(result.at(0).field( "fingerprint_left_thumb" ).value().toByteArray());
               image2 = QByteArray::fromBase64(result.at(0).field( "fingerprint_right_thumb" ).value().toByteArray());

               qDebug() << image1.isEmpty();

           }
           db.connClosed();
           emit member_fingerprints_get_success();
        }
        else
        {
            qDebug() << "api::getCapturedFingerprint() - Member fingerprint retrieval failed";
            db.connClosed();
            emit member_fingerprints_get_failure();
        }
    }
    else
    {
        qDebug() << "api::getCapturedFingerprint() - DB Connection failed";
    }

    emit member_fingerprints_get_failure();
}

void api::getCapturedPortrait(QString id, AdminMode mode)
{
    transmission_mode_ = GET_MEMBER_PORTRAIT;

    /* setup the webservice LOGIN url */
    QUrl serviceUrl;
    if( mode == ADMINISTER_MEMBER )
        serviceUrl = QUrl( base_url_ + "/api/membersportrait/" + id);
    else
        serviceUrl = QUrl( base_url_ + "/api/usersportrait/" + id);

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

void api::getCapturedPortraitFromDB(QString member_id, QByteArray &image)
{
    /* init database */
    Database db;

    /* Declare & sanitize db fields */
    QString table = "member_portrait";
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
           emit member_portrait_get_success();
        }
        else
        {
            qDebug() << "api::getCapturedPortrait() - Member portrait retrieval failed";
            db.connClosed();
        }
    }
    else
    {
        qDebug() << "api::getCapturedPortrait() - DB Connection failed";
    }
    emit member_portrait_get_failure();
}

void api::multiPostReplyFinished()
{
    qDebug() << "api::multiPostReplyFinished() ";
}

void api::uploadProgress(qint64 value1, qint64 value2)
{
    if( value1 == 0 && value2 == 0)
    {
        qDebug() << "---------Finished-----------" << endl;
    }
    else
    {
        qDebug() << "---------Uploaded-----------" <<
                    value1 << " of " << value2;
    }

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

        if( statusCode >= 200 && statusCode <= 309)
        {
            switch( transmission_mode_ )
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

                case GET_MEMBER_PORTRAIT:
                    qDebug() << "api::replyFinished() - Response" << endl;

                    jsonSuccess = jsonObject[ "success" ].toObject();
                    qDebug() << jsonObject << endl;
                    qDebug() << jsonSuccess.value("created_at").toString() << endl;
                    emit member_portrait_details_found(jsonObject);
                break;

                case GET_MEMBER_FINGERPRINTS:
                    qDebug() << "api::replyFinished() - Response" << endl;

                    jsonSuccess = jsonObject[ "success" ].toObject();
                    qDebug() << jsonObject << endl;
                    qDebug() << jsonSuccess.value("created_at").toString() << endl;
                    emit member_fingerprint_details_found(jsonObject);
                break;

                case GET_MEMBER_DETAILS:
                    qDebug() << "api::replyFinished() - Response" << endl;

                    jsonSuccess = jsonObject[ "success" ].toObject();
                    qDebug() << jsonObject << endl;
                    qDebug() << jsonSuccess.value("created_at").toString() << endl;
                    emit member_details_found(jsonObject);
                break;

                case POST_MEMBER_FINGERPRINTS:
                    qDebug() << "api::replyFinished() - Response" << endl;

                    jsonSuccess = jsonObject[ "success" ].toObject();
                    qDebug() << jsonObject << endl;
                    qDebug() << jsonSuccess.value("created_at").toString() << endl;
                    emit member_fingerprints_post_success();
                break;

                case POST_MEMBER_PORTRAIT:
                    qDebug() << "api::replyFinished() - Response" << endl;

                    jsonSuccess = jsonObject[ "success" ].toObject();
                    qDebug() << jsonObject << endl;
                    qDebug() << jsonSuccess.value("created_at").toString() << endl;
                    emit member_portrait_post_success();
                break;
            }
        }
        else
        {
            switch( transmission_mode_ )
            {
                case AUTH: emit auth_failed();
                break;
                case GET_MEMBER_FINGERPRINTS: emit member_fingerprint_details_not_found();
                break;
                case GET_MEMBER_PORTRAIT: emit member_portrait_details_not_found();
                break;
                case GET_MEMBER_DETAILS: emit member_details_not_found();
                break;
                case POST_MEMBER_FINGERPRINTS: emit member_fingerprints_post_failure();
                break;
                case POST_MEMBER_PORTRAIT: emit member_portrait_post_failure();
                break;
            }
            qDebug() << "api::replyFinished() - Unexpected Error occured with code: " << statusCode;
        }
        qDebug() << "api::replyFinished() - Finished";
   }
}
