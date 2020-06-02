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
                                   QByteArray image2, AdminMode mode,
                                  bool is_an_update)
{
    /* populate fields */
    QDateTime dt = QDateTime::currentDateTime();
    QString today = dt.toString("yyyy-MM-dd HH:mm:ss");
    qDebug() << "api::postCapturedFingerprint() - Date Today: " << today;

    transmission_mode_ = POST_MEMBER_FINGERPRINTS;

    /* setup the webservice LOGIN url */
    QUrl serviceUrl;
    QUrlQuery query;

    if( mode == ADMINISTER_MEMBER )
    {
        serviceUrl = QUrl( base_url_ + "/api/membersfingerprint" );

        query.addQueryItem( "id", id );
        query.addQueryItem( "fingerprint_left_thumb", "image1" );
        query.addQueryItem( "fingerprint_right_thumb", "image2" );
        query.addQueryItem( "comment", "member fingerprint captured" );
        /*query.addQueryItem( "created_at", today );
        query.addQueryItem( "updated_at", today );*/
    }
    else
    {
        serviceUrl = QUrl( base_url_ + "/api/usersfingerprint" );

        query.addQueryItem( "user_id", id );
        query.addQueryItem( "fingerprint", image1 );
        query.addQueryItem( "comments", "member fingerprint captured" );
        query.addQueryItem( "created_at", today );
        query.addQueryItem( "updated_at", today );
    }


    QByteArray postData;
    /* actual data to be posted */
    postData = query.toString(QUrl::FullyEncoded).toUtf8();

    /* print url */
    qDebug() << "api::authenticateUser() - Post Data ";
    qDebug() << postData;

    /* set format */
    QString sAuth = "Bearer " + auth_token_;
    QByteArray bAuth = QByteArray::fromStdString(sAuth.toStdString());

    QNetworkRequest networkRequest(serviceUrl);
    networkRequest.setRawHeader( "Authorization", bAuth );
    networkRequest.setRawHeader( "Accept", "application/json");
    networkRequest.setHeader( QNetworkRequest::ContentTypeHeader, \
                              "application/x-www-form-urlencoded");

    /* post login request */
    manager_->post( networkRequest, postData );
}

void api::postCapturedPortrait(QString id, QByteArray image,
                               AdminMode mode, bool is_an_update)
{
    /* populate fields */
    QDateTime dt = QDateTime::currentDateTime();
    QString today = dt.toString("yyyy-MM-dd HH:mm:ss");
    qDebug() << "api::postCapturedFingerprint() - Date Today: " << today;

    transmission_mode_ = POST_MEMBER_PORTRAIT;

    /* setup the webservice LOGIN url */
    QUrl serviceUrl;
    QUrlQuery query;

    if( mode == ADMINISTER_MEMBER )
    {
        serviceUrl = QUrl( base_url_ + "/api/membersportrait/?" );

        query.addQueryItem( "member_id", id );
        query.addQueryItem( "portrait", image );
        query.addQueryItem( "comments", "member portrait captured" );
        query.addQueryItem( "created_at", today );
        query.addQueryItem( "updated_at", today );
    }
    else
    {
        serviceUrl = QUrl( base_url_ + "/api/usersportrait/?" );

        query.addQueryItem( "user_id", id );
        query.addQueryItem( "portrait", image );
        query.addQueryItem( "comments", "user portrait captured" );
        query.addQueryItem( "created_at", today );
        query.addQueryItem( "updated_at", today );
    }

    QByteArray postData;
    /* actual data to be posted */
    postData = query.toString(QUrl::FullyEncoded).toUtf8();

    /* print url */
    qDebug() << "api::authenticateUser() - Post Data ";
    qDebug() << postData;

    /* set format */
    QString sAuth = "Bearer " + auth_token_;
    QByteArray bAuth = QByteArray::fromStdString(sAuth.toStdString());

    /* set format */
    QNetworkRequest networkRequest(serviceUrl);
    networkRequest.setHeader( QNetworkRequest::ContentTypeHeader, \
                              "application/x-www-form-urlencoded");

    /* post login request */
    manager_->post( networkRequest, postData );
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

        if( statusCode >= 200 && statusCode <= 226)
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
