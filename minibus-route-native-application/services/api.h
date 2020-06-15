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

    void isUserRegistered(QString id);

    void postCapturedFingerprint(QString id, QByteArray image1,
                                 QByteArray image2, AdminMode mode,
                                 bool is_an_update);

    void postCapturedPortrait(QString id, QByteArray image,
                              AdminMode mode, bool is_an_update);

    void getCapturedFingerprint(QString id, AdminMode mode);

    void getCapturedPortrait(QString id, AdminMode mode);

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
    void member_details_found(QJsonObject &);
    void member_details_not_found();
    void member_portrait_details_found(QJsonObject &);
    void member_portrait_details_not_found();
    void member_fingerprint_details_found(QJsonObject &);
    void member_fingerprint_details_not_found();
    void member_fingerprints_post_success();
    void member_fingerprints_post_failure();
    void member_portrait_post_success();
    void member_portrait_post_failure();

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
};

#endif // API_H
