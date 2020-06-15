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
    /* populate fields */
    QDateTime dt = QDateTime::currentDateTime();
    QString today = dt.toString("yyyy-MM-dd HH:mm:ss");
    qDebug() << "api::postCapturedFingerprint() - Date Today: " << today;

    /* write files to device */
    QFile *file1 = new QFile("image1.png");
    file1->open(QIODevice::WriteOnly);
    file1->write(image1);
    file1->close();

    QFile *file2 = new QFile("image2.png");
    file2->open(QIODevice::WriteOnly);
    file2->write(image2);
    file2->close();

    transmission_mode_ = POST_MEMBER_FINGERPRINTS;

    /* setup the webservice LOGIN url */
    QUrl serviceUrl;
    QUrlQuery query;

    /* set token */
    QString sAuth = "Bearer " + auth_token_;
    QByteArray bAuth = QByteArray::fromStdString(sAuth.toStdString());

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    qDebug() << "api::postCapturedFingerprint() - 1: ";
    if( mode == ADMINISTER_MEMBER )
    {
        serviceUrl = QUrl( base_url_ + "/api/membersfingerprint" );

        QHttpPart authPart;
        authPart.setRawHeader( "Authorization", bAuth );

        QHttpPart acceptPart;
        acceptPart.setRawHeader( "Accept", "application/json");

        QHttpPart contentTypePart;
        contentTypePart.setHeader( QNetworkRequest::ContentTypeHeader, \
                                      "application/x-www-form-urlencoded");

        QHttpPart idPart;
        idPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                         QVariant("form-data; name=\"member_id\""));
        id = "9";
        idPart.setBody(id.toLatin1());
        qDebug() << "Member ID: " << id;

        qDebug() << "api::postCapturedFingerprint() - 2: ";

        QHttpPart image1Part;
        image1Part.setHeader(QNetworkRequest::ContentDispositionHeader,
                            QVariant("form-data; name=\"fingerprint_left_thumb\""));

        qDebug() << "api::postCapturedFingerprint() - :3 ";

        QHttpPart image2Part;
        image2Part.setHeader(QNetworkRequest::ContentDispositionHeader,
                            QVariant("form-data; name=\"fingerprint_right_thumb\""));

        qDebug() << "api::postCapturedFingerprint() - 4: ";

        /* read files from device */
        file1->open(QIODevice::ReadOnly);
        image1Part.setBodyDevice(file1);
        file1->setParent(multiPart);

        qDebug() << "api::postCapturedFingerprint() - 5: ";
        file2->open(QIODevice::ReadOnly);
        image2Part.setBodyDevice(file2);
        file2->setParent(multiPart);

        qDebug() << "api::postCapturedFingerprint() - 6: ";

        /* add form -data */
        multiPart->append(authPart);
        multiPart->append(acceptPart);
        multiPart->append(contentTypePart);
        multiPart->append(idPart);
        multiPart->append(image1Part);
        multiPart->append(image2Part);
        qDebug() << "api::postCapturedFingerprint() - 7: ";
    }
    else
    {
        serviceUrl = QUrl( base_url_ + "/api/usersfingerprint?" );

        query.addQueryItem( "user_id", id );
        query.addQueryItem( "fingerprint", image1 );
        query.addQueryItem( "comments", "member fingerprint captured" );
        query.addQueryItem( "created_at", today );
        query.addQueryItem( "updated_at", today );
    }

    /* set request */
    QNetworkRequest networkRequest(serviceUrl);

    /* post login request */
    if( is_an_update )
    {
        manager_->put( networkRequest, multiPart );
    }
    else
    {
        qDebug() << "api::postCapturedFingerprint() - 8: ";
        reply_ = manager_->post( networkRequest, multiPart );
        multiPart->setParent(reply_);
        linkReply();

        qDebug() << "api::postCapturedFingerprint() - 9: ";
    }
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
