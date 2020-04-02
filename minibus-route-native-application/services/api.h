#ifndef API_H
#define API_H

#include <QObject>
#include <database.h>

class api : public QObject
{
    Q_OBJECT
public:
    explicit api(QObject *parent = nullptr);

    /*!
     * \brief User Authentication
     * \return
     */
    bool authenticateUser();

    /*!
     * \brief Member Verification
     * \return
     */
    bool isMemberRegistered();

    bool postCapturedFingerprint(QString member_id, QByteArray image);

    bool postCapturedPortrait(QString member_id, QByteArray image);

    void getCapturedFingerprint(QByteArray &image);

    void getCapturedPortrait(QByteArray &image);


signals:

};

#endif // API_H
