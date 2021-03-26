#include "api.h"
#include <QFile>
#include <QProcessEnvironment>

api *api::api_instance_ = 0;

api::api(QObject *parent) : QObject(parent)
{
    process_ = new QProcess(this);

    /* Set HTTPS URL connection */
    initConnection("ptrms-test.csir.co.za");
}

void api::initConnection(QString address)
{
    /* set url */
    base_url_ = address;
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

    QString path = QDir::currentPath()
                    + "/scripts/sc_auth.py";

    QString program( QDir::currentPath()
                     + "/resources/python39/python" );

    QStringList params =
            QStringList() << path
                          << this->base_url_
                          << username
                          << password;

    process_->start(program, params);

    if( process_->waitForFinished() )
    {
        qDebug() << "Process Completed...1";
        QFile out( QDir::currentPath() + "/scripts/out.txt" );
        if( out.open(QIODevice::ReadOnly) )
        {
            auth_token_ = out.readAll();
            qDebug() << "Auth Token: " << auth_token_;
            out.close();
            qDebug() << "Process Completed...2";

            /* close process */
            process_->kill();

            /* remove file */
            out.remove( QDir::currentPath() + "/scripts/out.txt" );

            if( auth_token_.isEmpty() ) { emit auth_failed(); }
            else { emit auth_successful(); }
        }
        else
        {
            qDebug() << "api::authenticateUser() - Output file could not be found";
            emit auth_failed();
        }
    }
    else
    {
        qDebug() << "api::authenticateUser() - User Authentication failed...";
        emit auth_failed();
    }
}

void api::isMemberRegistered(QString id)
{
    transmission_mode_ = GET_MEMBER_DETAILS;

    QString subject_type = "members";

    QString path = QDir::currentPath()
                    + "/scripts/sc_is_subject_registered.py";

    QString program( QDir::currentPath()
                     + "/resources/python39/python" );

    QStringList params =
            QStringList() << path
                          << this->base_url_
                          << auth_token_
                          << subject_type
                          << id;

    process_->start(program, params);

    if( process_->waitForFinished() )
    {
        QFile out( QDir::currentPath() + "/scripts/out.txt" );
        if( out.open(QIODevice::ReadOnly) )
        {
            /* close process */
            process_->kill();

            /* Process Response */
            processResponse( out.readAll() );
            out.close();

            /* remove file */
            out.remove( QDir::currentPath() + "/scripts/out.txt" );
        }
        else
        {
            qDebug() << "api::isMemberRegistered() - Member output file could not be found";
            emit details_not_found();
        }
    }
    else
    {
        qDebug() << "api::isMemberRegistered() - Member Details fetch failed...";
        emit details_not_found();
    }

}

void api::isUserRegistered(QString id)
{
    transmission_mode_ = GET_USER_DETAILS;

    /* setup the webservice LOGIN url */
    QUrl serviceUrl = QUrl( base_url_ + "/api/users/" + id);

    /* To be completed */

}

void api::isEmployeeRegistered(QString id)
{
    transmission_mode_ = GET_EMPLOYEE_DETAILS;

    QString subject_type = "employees";

    QString path = QDir::currentPath()
                    + "/scripts/sc_is_subject_registered.py";

    QString program( QDir::currentPath()
                     + "/resources/python39/python" );

    QStringList params =
            QStringList() << path
                          << this->base_url_
                          << auth_token_
                          << subject_type
                          << id;

    process_->start(program, params);

    if( process_->waitForFinished() )
    {
        qDebug() << "Process Completed...1";
        QFile out( QDir::currentPath() + "/scripts/out.txt" );
        if( out.open(QIODevice::ReadOnly) )
        {
            /* close process */
            process_->kill();

            /* Process Response */
            processResponse( out.readAll() );
            out.close();

            /* remove file */
            out.remove( QDir::currentPath() + "/scripts/out.txt" );
        }
        else
        {
            qDebug() << "api::isEmployeeRegistered() - Employee output file could not be found";
            emit employee_details_not_found();
        }
    }
    else
    {
        qDebug() << "api::isEmployeeRegistered() - Employee Details fetch failed...";
        emit employee_details_not_found();
    }
}

/* HTTPS @GET, url: ../api/military_veterans/{id} */
void api::isMilitaryVeteranRegistered(QString id)
{
    transmission_mode_ = GET_MILITARY_VETERAN_DETAILS;

    QString subject_type = "military_veterans";

    QString path = QDir::currentPath()
                    + "/scripts/sc_is_subject_registered.py";

    QString program( QDir::currentPath()
                     + "/resources/python39/python" );

    QStringList params =
            QStringList() << path
                          << this->base_url_
                          << auth_token_
                          << subject_type
                          << id;

    process_->start(program, params);

    if( process_->waitForFinished() )
    {
        qDebug() << "Process Completed...1";
        QFile out( QDir::currentPath() + "/scripts/out.txt" );
        if( out.open(QIODevice::ReadOnly) )
        {
            /* close process */
            process_->kill();

            /* Process Response */
            processResponse( out.readAll() );
            out.close();

            /* remove file */
            out.remove( QDir::currentPath() + "/scripts/out.txt" );
        }
        else
        {
            qDebug() << "api::isMilitaryVeteranRegistered() - Output file could not be found";
            emit details_not_found();
        }
    }
    else
    {
        qDebug() << "api::isMilitaryVeteranRegistered() - MV Details fetch failed...";
        emit details_not_found();
    }

}

void api::postCapturedFingerprint(QString id, AdminMode mode,
                             QString image_file_path1,
                             QString image_file_path2,
                                  bool is_an_update)
{
    QString path = "";
    QStringList params;

    if( mode == ADMINISTER_MEMBER )
    {
        transmission_mode_ = POST_MEMBER_FINGERPRINTS;
        path = QDir::currentPath()
                + "/scripts/sc_upload_member_fingerprint.py";
        params = QStringList() << path
                               << this->base_url_
                               << auth_token_
                               << image_file_path1
                               << image_file_path2
                               << id;
    }
    else
    {
        if( mode == ADMINISTER_EMPLOYEE )
        {
            transmission_mode_ = POST_EMPLOYEE_FINGERPRINTS;
            path = QDir::currentPath()
                    + "/scripts/sc_upload_employee_fingerprint.py";
        }
        else if( mode == ADMINISTER_MILITARY_VETERAN )
        {
            transmission_mode_ = POST_MILITARY_VETERAN_FINGERPRINTS;
            path = QDir::currentPath()
                    + "/scripts/sc_upload_mv_fingerprint.py";
        }
        else
        {
            transmission_mode_ = POST_USER_FINGERPRINTS;
            path = QDir::currentPath()
                    + "/scripts/sc_upload_user_fingerprint.py";
        }

        params = QStringList() << path
                              << this->base_url_
                              << auth_token_
                              << image_file_path1
                              << id;

        qDebug() << " ************** ";
        qDebug() << params.first() << endl;
        qDebug() << params[1] << endl;
        qDebug() << params[3] << endl;
        qDebug() << params.last() << endl;
    }

    QString program( QDir::currentPath()
                     + "/resources/python39/python" );


    process_->start(program, params);

    if( process_->waitForFinished() )
    {
        qDebug() << "Process Completed...1";
        QFile out( QDir::currentPath() + "/scripts/out.txt" );
        if( out.open(QIODevice::ReadOnly) )
        {
            /* close process */
            process_->kill();

            /* Process Response */
            processResponse( out.readAll() );
            out.close();

            /* remove file */
            out.remove( QDir::currentPath() + "/scripts/out.txt" );
        }
        else
        {
            qDebug() << "api::postCapturedFingerprint() - "
            "Fingerprint Response output file could not be found";
            emit fingerprints_post_failure();
        }
    }
    else
    {
        qDebug() << "api::postCapturedFingerprint() - Post failed...";
        emit fingerprints_post_failure();
    }

}

void api::postCapturedFingerprintDB(QString id, QByteArray image1,
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

void api::postCapturedPortraitDB(QString id, QByteArray image,
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
    else if ( mode == ADMINISTER_EMPLOYEE ){
        serviceUrl = QUrl( base_url_ + "/api/employees/" + id);
    }
    else if ( mode == ADMINISTER_MILITARY_VETERAN )
    {
        serviceUrl = QUrl( base_url_ + "/api/militaryveteranfingerprint/" + id);
    }
    else{
        serviceUrl = QUrl( base_url_ + "/api/usersfingerprint/" + id);
    }

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

void api::processResponse(QString response)
{
    QJsonDocument json_response = QJsonDocument::fromJson( response.toUtf8() );

    if( !response.isEmpty() )
    {
        QJsonObject jsonObject = json_response.object();
        QJsonObject jsonSuccess;

        switch( transmission_mode_ )
        {
            case AUTH:
                jsonSuccess = jsonObject[ "success" ].toObject();
                auth_token_ = jsonSuccess.value("token").toString();

                qDebug() << auth_token_ << endl;
                emit auth_successful();
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

            case POST_MILITARY_VETERAN_FINGERPRINTS:
                jsonSuccess = jsonObject[ "data" ].toObject();
                qDebug() << jsonObject << endl;
                qDebug() << jsonSuccess.value("created_at").toString() << endl;
                emit fingerprints_post_success();
            break;

            case GET_MILITARY_VETERAN_DETAILS:
                QString bytes = response;
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
            case POST_MILITARY_VETERAN_FINGERPRINTS: emit fingerprints_post_failure();
            break;
        }
    }

    qDebug() << "api::processResponse() - Finished";

}
