#ifndef API_H
#define API_H

#include <QObject>
#include <database.h>

#include <QVector>
#include <QSqlField>
#include <QSqlRecord>


class api : public QObject
{
    Q_OBJECT
public:
    explicit api(QObject *parent = nullptr);

    /*!
     * \brief authenticateUser
     * \param username
     * \param password
     * \return
     */
    bool authenticateUser(QString username, QString password);

    /*!
     * \brief Member Verification
     * \return
     */
    bool isMemberRegistered(QString id_type, QString id, QVector<QSqlRecord> &result);

    bool postCapturedFingerprint(QString member_id, QByteArray image);

    bool postCapturedPortrait(QString member_id, QByteArray image);

    bool getCapturedFingerprint(QString member_id, QByteArray &image);

    bool getCapturedPortrait(QString member_id, QByteArray &image);


signals:

};

#endif // API_H
