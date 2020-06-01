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

    bool postCapturedFingerprint(QString member_id, QByteArray image, bool is_an_update);

    bool postCapturedPortrait(QString member_id, QByteArray image, bool is_an_update);

    bool getCapturedFingerprint(QString member_id, QByteArray &image);

    bool getCapturedPortrait(QString member_id, QByteArray &image);

public slots:
    /*!
     * \brief replyFinished
     * \param networkReply
     */
    void replyFinished(QNetworkReply *networkReply);


signals:

    void auth_successful();
    void auth_failed();
    void member_details_found(QJsonObject &);
    void member_details_not_found();

private:

    explicit api(QObject *parent = nullptr);

    static api *api_instance_;

    QNetworkAccessManager *manager_;
    QString base_url_;
    QUrl *urlookup_;

    QJsonDocument json_response_;
    QString auth_token_;
    bool is_authenticating_;
    TransmissionMode mode_;


    /*!
     * \brief initConnection
     * \param address (e.g. 127.0.0.1)
     * \param port (e.g. 8000)
     */
    void initConnection(QString address, int port);


};

#endif // API_H
