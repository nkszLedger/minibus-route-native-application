#ifndef API_H
#define API_H

#include <QObject>
#include <global.h>
#include <QSqlRecord>
#include <database.h>
#include <QDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QMessageBox>

class api : public QObject
{
    Q_OBJECT
public:

    /*!
    * \brief instance
    * \return
    */
    static api *instance()
    {
      if (!api_instance_)
        api_instance_ = new api;
      return api_instance_;
    }

    /*!
     * \brief authenticateUser
     * \param username
     * \param password
     * \return
     */
    void authenticateUser(QString username, QString password);

    /*!
     * \brief Member Verification
     * \return
     */
    void isMemberRegistered(QString id);
    void isEmployeeRegistered(QString id);
    void isMilitaryVeteranRegistered(QString id);
    void isUserRegistered(QString id);

    void postCapturedFingerprint(QString id, QByteArray image1,
                                 QByteArray image2, QString table,
                                 AdminMode mode, bool is_an_update);

    void postCapturedPortrait(QString id, QByteArray image,
                              AdminMode mode, QString table,
                              bool is_an_update);

    void getCapturedFingerprint(QString id, AdminMode mode);

    void getCapturedPortrait(QString id, AdminMode mode);

    void getCapturedFingerprintFromDB(QString id,
                                      QByteArray &image1,
                                      QByteArray &image2,
                                      AdminMode mode,
                                      QString table);

    void getCapturedPortraitFromDB(QString id,
                                   QByteArray &image,
                                   AdminMode mode,
                                   QString table);

public slots:
    /*!
     * \brief replyFinished
     * \param networkReply
     */
    void replyFinished(QNetworkReply *networkReply);
    void multiPostReplyFinished();
    void uploadProgress(qint64 value1, qint64 value2);
signals:

    void auth_successful();
    void auth_failed();

    void employee_details_found(QJsonObject &);
    void employee_details_not_found();

    void details_found(QJsonObject &);
    void details_not_found();

    void portrait_details_found(QJsonObject &);
    void portrait_details_not_found();

    void fingerprint_details_found(QJsonObject &);
    void fingerprint_details_not_found();

    void fingerprints_get_success();
    void fingerprints_get_failure();

    void fingerprints_post_success();
    void fingerprints_post_failure();

    void portrait_post_success();
    void portrait_post_failure();

    void portrait_get_success();
    void portrait_get_failure();

private:

    explicit api(QObject *parent = nullptr);

    static api *api_instance_;

    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
    QString base_url_;
    QUrl *urlookup_;

    QJsonDocument json_response_;
    QString auth_token_;
    bool is_authenticating_;
    TransmissionMode transmission_mode_;


    /*!
     * \brief initConnection
     * \param address (e.g. 127.0.0.1)
     * \param port (e.g. 8000)
     */
    void initConnection(QString address, int port);


    void linkReply();
    void showMessage(QString title, QString Message, QMessageBox::Icon type);
    void attemptConnection();
};

#endif // API_H
