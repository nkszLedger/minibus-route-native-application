#include "api.h"
#include <QFile>
#include <QHttpPart>
#include <QVariant>


api *api::api_instance_ = 0;

api::api(QObject *parent) : QObject(parent)
{
    socket_ = new QSslSocket(this);

    manager_ = new QNetworkAccessManager(this);

    qDebug() << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << QSslSocket::supportsSsl();
    qDebug() << QSslSocket::sslLibraryVersionString();

    connect(manager_, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    connect(manager_, SIGNAL(sslErrors(QNetworkReply *,
                     const QList<QSslError> &)),
            this, SLOT(sslErrors(QNetworkReply*
                     const QList<QSslError> &)));

    initConnection("ptrms-test.csir.co.za", 443); // 127.0.0.1
}

void api::initConnection(QString address, int port)
{
    /* set url */
    base_url_ = "https://" + address; //+ ":" + QString::number(port);

    /* connect to host via http on port */
    urlookup_ = new QUrl( base_url_ );

    /* connect to share */
    /*manager_->connectToHost(address, port);*/
}

bool api::attemptConnection()
{    
    socket_->connectToHostEncrypted("ptrms-test.csir.co.za", 443);

    if (!socket_->waitForEncrypted())
    {
        showMessage("Connection Status", "Web System offline",
                    QMessageBox::Critical);
        return false;
    }
    return true;
}

void api::showMessage(QString title, QString message,
                  QMessageBox::Icon type)
{
    QMessageBox message_box;
    message_box.setWindowOpacity(50);
    message_box.setWindowTitle(title);
    message_box.setStyleSheet("QLabel{ font-weight: plain; font-size: 14px; } \
                                 QPushButton{ width:125px; height:10; font-size: 14px; }");
    message_box.setStandardButtons(QMessageBox::Ok);

    message_box.setIcon(type);
    message_box.setText(message);

    message_box.exec();
}

void api::authenticateUser(QString username, QString password)
{
    transmission_mode_ = AUTH;

    /* test connection */
    if( attemptConnection() )
    {
        QString str_credentials= "email=" + username +"&password=" + password;
        QString str_post = "POST /api/login?" + str_credentials + " HTTP/1.0\r\n\r\n";

        socket_->write( str_post.toStdString().c_str() );

        while (socket_->waitForReadyRead())
        {
            QString response = socket_->readAll().data();

            if( response.split("error").length() > 1 )
            {
                emit auth_failed();
            }
            else
            {
                QStringList str_split_response = response.split("\"");
                qDebug() << "Token: " << str_split_response[str_split_response.length()-2];

                auth_token_ = str_split_response[str_split_response.length()-2];
                emit auth_successful();
            }
        }
    }
    else
    {
        emit auth_failed();
    }
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

void api::isEmployeeRegistered(QString id)
{
    transmission_mode_ = GET_EMPLOYEE_DETAILS;

    /* setup the webservice LOGIN url */
    QUrl serviceUrl = QUrl( base_url_ + "/api/employees/" + id);

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

void api::isMilitaryVeteranRegistered(QString id)
{
    transmission_mode_ = GET_MILITARY_VETERAN_DETAILS;

    /* setup the webservice LOGIN url */
    QUrl serviceUrl = QUrl( base_url_ + "/api/military_veterans/" + id);

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
                                   QByteArray image2, QString table,
                                   AdminMode mode,
                                   bool is_an_update)
{
    transmission_mode_ = POST_MILITARY_VETERAN_FINGERPRINTS;

    /* init database */
    Database db;

    /* Declare & sanitize db fields */

    /* populate fields */
    QDateTime dt = QDateTime::currentDateTime();
    QString today = dt.toString("yyyy-MM-dd HH:mm:ss");
    qDebug() << "api::postCapturedFingerprint() - Date Today: " << today;


    if( db.connOpen() )
    {
        if( is_an_update )
        {
            if( mode == ADMINISTER_MEMBER )
            {
                if( db.updateTemplate( table, "fingerprint2", id, today, image1, image2 ))
                {
                    qDebug() << "api::postCapturedFingerprint() - MEMBER Fingerprint UPDATE successful";
                    emit fingerprints_post_success();
                }
                else
                {
                    qDebug() << "api::postCapturedFingerprint() - MEMBER Fingerprint UPDATE failed";
                    emit fingerprints_post_failure();

                }
            }
            else if( mode == ADMINISTER_MILITARY_VETERAN )
            {
                if( db.updateTemplate( table, "fingerprint_military_veteran", id, today, image1 ))
                {
                    qDebug() << "api::postCapturedFingerprint() - MILITARY VETERAN Fingerprint UPDATE successful";
                    emit fingerprints_post_success();
                }
                else
                {
                    qDebug() << "api::postCapturedFingerprint() - MILITARY VETERAN Fingerprint UPDATE failed";
                    emit fingerprints_post_failure();
                }
            }
            else
            {
                if( db.updateTemplate( table, "fingerprint", id, today, image1 ))
                {
                    qDebug() << "api::postCapturedFingerprint() - EMPLOYEE Fingerprint UPDATE successful";
                    emit fingerprints_post_success();
                }
                else
                {
                    qDebug() << "api::postCapturedFingerprint() - EMPLOYEE Fingerprint UPDATE failed";
                    emit fingerprints_post_failure();
                }
            }


        }
        else
        {
            if( mode == ADMINISTER_MEMBER )
            {
                if( db.insertTemplate(table, "fingerprint2", id, today, today, image1, image2) )
                {
                   qDebug() << "api::postCapturedFingerprint() - MEMBER Fingerprint POST successful";
                   emit fingerprints_post_success();
                }
                else
                {
                    qDebug() << "api::postCapturedFingerprint() - MEMBER Fingerprint POST failed";
                    emit fingerprints_post_failure();
                }
            }
            else if( mode == ADMINISTER_MILITARY_VETERAN )
            {
                if( db.insertTemplate(table, "fingerprint_military_veteran", id, today, today, image1) )
                {
                   qDebug() << "api::postCapturedFingerprint() - MILITARY VETERAN Fingerprint POST successful";
                   emit fingerprints_post_success();
                }
                else
                {
                    qDebug() << "api::postCapturedFingerprint() - MILITARY VETERAN Fingerprint POST failed";
                    emit fingerprints_post_failure();
                }
            }
            else
            {
                if( db.insertTemplate(table, "fingerprint", id, today, today, image1) )
                {
                   qDebug() << "api::postCapturedFingerprint() - EMPLOYEE Fingerprint POST successful";
                   emit fingerprints_post_success();
                }
                else
                {
                    qDebug() << "api::postCapturedFingerprint() - EMPLOYEE Fingerprint POST failed";
                    emit fingerprints_post_failure();
                }
            }
        }
    }

    db.connClosed();
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
                               AdminMode mode, QString table,
                               bool is_an_update)
{
    /* populate fields */
    QDateTime dt = QDateTime::currentDateTime();
    QString today = dt.toString("yyyy-MM-dd HH:mm:ss");
    qDebug() << "api::postCapturedPortrait() - Date Today: " << today;

    transmission_mode_ = POST_MEMBER_PORTRAIT;

    /* init database */
    Database db;

    /* Declare & sanitize db fields */
    if( db.connOpen() )
    {
        if( is_an_update )
        {
            if( mode == ADMINISTER_MEMBER )
            {
                if( db.updateTemplate( table, "portrait", id, today, image ))
                {
                    qDebug() << "api::postCapturedPortrait() - MEMBER Portrait UPDATE successful";
                    emit portrait_post_success();
                }
                else
                {
                    qDebug() << "api::postCapturedPortrait() - MEMBER Portrait UPDATE failed";
                    emit portrait_post_failure();
                }
            }
            else if( mode == ADMINISTER_MILITARY_VETERAN )
            {
                if( db.updateTemplate( table, "portrait_military_veteran", id, today, image ))
                {
                    qDebug() << "api::postCapturedPortrait() - MILITARY VETERAN Portrait UPDATE successful";
                    emit portrait_post_success();
                }
                else
                {
                    qDebug() << "api::postCapturedPortrait() - MILITARY VETERAN Portrait UPDATE failed";
                    emit portrait_post_failure();
                }
            }
            else
            {
                if( db.updateTemplate( table, "", id, today, image ))
                {
                    qDebug() << "api::postCapturedPortrait() - EMPLOYEE Portrait UPDATE successful";
                    emit portrait_post_success();
                }
                else
                {
                    qDebug() << "api::postCapturedPortrait() - EMPLOYEE Portrait UPDATE failed";
                    emit portrait_post_failure();
                }
            }
        }
        else
        {
            if( mode == ADMINISTER_MEMBER )
            {
                if( db.insertTemplate(table, "portrait", id, today, today, image) )
                {
                   qDebug() << "api::postCapturedPortrait() - MEMBER Portrait POST successful";
                   emit portrait_post_success();
                }
                else
                {
                    qDebug() << "api::postCapturedPortrait() - MEMBER Portrait POST failed";
                    emit portrait_post_failure();
                }
            }
            else if( mode == ADMINISTER_MILITARY_VETERAN )
            {
                if( db.insertTemplate(table, "portrait_military_veteran", id, today, today, image) )
                {
                   qDebug() << "api::postCapturedPortrait() - MILITARY VETERAN Portrait POST successful";
                   emit portrait_post_success();
                }
                else
                {
                    qDebug() << "api::postCapturedPortrait() - MILITARY VETERAN Portrait POST failed";
                    emit portrait_post_failure();
                }
            }
            else
            {
                if( db.insertTemplate(table, "", id, today, today, image) )
                {
                   qDebug() << "api::postCapturedPortrait() - EMPLOYEE Portrait POST successful";
                   emit portrait_post_success();
                }
                else
                {
                    qDebug() << "api::postCapturedPortrait() - EMPLOYEE Portrait POST failed";
                    emit portrait_post_failure();
                }
            }
        }
    }
    else
    {
        qDebug() << "api::postCapturedPortrait() - DB Connection failed";
        emit portrait_post_failure();
    }

    db.connClosed();
}

void api::getCapturedFingerprint(QString id, AdminMode mode)
{
    transmission_mode_ = GET_MEMBER_FINGERPRINTS;

    /* setup the webservice LOGIN url */
    QUrl serviceUrl;
    if( mode == ADMINISTER_MEMBER ){
        serviceUrl = QUrl( base_url_ + "/api/membersfingerprint/" + id);
    }
    /*else if ( mode == ADMINISTER_EMPLOYEE ){
        serviceUrl = QUrl( base_url_ + "/api/employees/" + id);
    }
    else if ( mode == ADMINISTER_MILITARY_VETERAN ){
        serviceUrl = QUrl( base_url_ + "/api/military_veterans/" + id);
    }*/
    else{
        serviceUrl = QUrl( base_url_ + "/api/usersfingerprint/" + id);
    }
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

void api::getCapturedFingerprintFromDB(QString id,
                                       QByteArray &image1,
                                       QByteArray &image2,
                                       AdminMode mode,
                                       QString table)
{
    /* init database */
    Database db;

    /* Declare & sanitize db fields */
    QVector<QString> select_columns;
    QVector<QString> column_list;
    QVector<QString> value_list;
    QVector<QSqlRecord> result;

    if( mode == ADMINISTER_MEMBER )
    {
        select_columns = { "fingerprint_left_thumb",
                            "fingerprint_right_thumb"
                         };

        column_list = {"member_id"};
        value_list = { id };
    }
    else if( mode == ADMINISTER_MILITARY_VETERAN )
    {
        select_columns = { "fingerprint" };
        column_list = {"military_veteran_id"};
        value_list = { id };
    }
    else
    {
        select_columns = { "fingerprint" };
        column_list = {"employee_id"};
        value_list = { id };
    }

    if( db.connOpen() )
    {
        if( db.select(table, select_columns, column_list, value_list, result) )
        {
           qDebug() << "api::getCapturedFingerprint() - Fingerprint retrieval request successful";
           if( !result.isEmpty() )
           {
               qDebug() << "api::getCapturedFingerprint() - Data found";

               if( mode == ADMINISTER_MEMBER )
               {
                   image1 = QByteArray::fromBase64(result.at(0).field( "fingerprint_left_thumb" ).value().toByteArray());
                   image2 = QByteArray::fromBase64(result.at(0).field( "fingerprint_right_thumb" ).value().toByteArray());
                   qDebug() << image1.isEmpty();
               }
               else
               {
                   image1 = QByteArray::fromBase64(result.at(0).field( "fingerprint" ).value().toByteArray());
                   qDebug() << image1.isEmpty();
               }
           }
           db.connClosed();
           emit fingerprints_get_success();
        }
        else
        {
            qDebug() << "api::getCapturedFingerprint() - Fingerprint retrieval request failed";
            db.connClosed();
            emit fingerprints_get_failure();
        }
    }
    else
    {
        qDebug() << "api::getCapturedFingerprint() - DB Connection failed";
    }

    emit fingerprints_get_failure();
}

void api::getCapturedPortrait(QString id, AdminMode mode)
{
    transmission_mode_ = GET_MEMBER_PORTRAIT;

    /* setup the webservice LOGIN url */
    QUrl serviceUrl;
    if( mode == ADMINISTER_MEMBER ) {
        serviceUrl = QUrl( base_url_ + "/api/membersportrait/" + id);
    }
    /*else if ( mode == ADMINISTER_EMPLOYEE ){
        serviceUrl = QUrl( base_url_ + "/api/employees/" + id);
    }
    else if ( mode == ADMINISTER_MILITARY_VETERAN ){
        serviceUrl = QUrl( base_url_ + "/api/military_veterans/" + id);
    }*/
    else {
        serviceUrl = QUrl( base_url_ + "/api/usersportrait/" + id);
    }
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

void api::getCapturedPortraitFromDB(QString id,
                                    QByteArray &image,
                                    AdminMode mode,
                                    QString table)
{
    /* init database */
    Database db;

    /* Declare & sanitize db fields */
    /* Declare & sanitize db fields */
    QVector<QString> select_columns;
    QVector<QString> column_list;
    QVector<QString> value_list;
    QVector<QSqlRecord> result;

    if( mode == ADMINISTER_MEMBER )
    {   column_list = {"member_id"}; }
    else if( mode == ADMINISTER_MILITARY_VETERAN )
    {   column_list = {"military_veteran_id"}; }
    else
    {   column_list = {"employee_id"}; }

    select_columns = { "portrait" };
    value_list = { id };


    if( db.connOpen() )
    {
        if( db.select(table, select_columns, column_list, value_list, result) )
        {
           qDebug() << "api::getCapturedPortrait() - Portrait retrieval successful";
           if( !result.isEmpty() )
           {
               image = QByteArray::fromBase64(result.at(0).field( "portrait" ).value().toByteArray());
           }

           db.connClosed();
           emit portrait_get_success();
        }
        else
        {
            qDebug() << "api::getCapturedPortrait() - Portrait retrieval failed";
            db.connClosed();
        }
    }
    else
    {
        qDebug() << "api::getCapturedPortrait() - DB Connection failed";
    }
    emit portrait_get_failure();
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

        if( statusCode == 200 )
        {
            switch( transmission_mode_ )
            {
                case AUTH:
                    /*jsonSuccess = jsonObject[ "success" ].toObject();
                    auth_token_ = jsonSuccess.value("token").toString();

                    qDebug() << auth_token_ << endl;
                    emit auth_successful();*/

                break;
                case GET_MEMBER_DETAILS:
                    qDebug() << "api::replyFinished() - Response" << endl;

                    jsonSuccess = jsonObject[ "success" ].toObject();
                    qDebug() << jsonObject << endl;
                    qDebug() << jsonSuccess.value("created_at").toString() << endl;
                    emit details_found(jsonObject);
                break;

                case GET_EMPLOYEE_DETAILS:

                    qDebug() << "api::replyFinished() - Response" << endl;

                    jsonSuccess = jsonObject[ "data" ].toObject();
                    qDebug() << jsonObject << endl;
                    qDebug() << jsonSuccess.value("created_at").toString() << endl;
                    emit employee_details_found(jsonObject);
                break;

                case GET_MILITARY_VETERAN_DETAILS:

                    qDebug() << "api::replyFinished() - Response" << endl;

                    jsonSuccess = jsonObject[ "data" ].toObject();
                    qDebug() << jsonObject << endl;
                    qDebug() << jsonSuccess.value("created_at").toString() << endl;
                    emit details_found(jsonObject);
                break;
            }
        }
        else
        {
            switch( transmission_mode_ )
            {
                case AUTH: emit auth_failed();
                break;
                case GET_MEMBER_DETAILS: emit details_not_found();
                break;
                case GET_EMPLOYEE_DETAILS: emit employee_details_not_found();
                break;
                case GET_MILITARY_VETERAN_DETAILS: emit details_not_found();
                break;
            }
            qDebug() << "api::replyFinished() - Unexpected Error occured with code: " << statusCode;
        }
        qDebug() << "api::replyFinished() - Finished";
   }
}

void api::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    qDebug() << errors;
}
