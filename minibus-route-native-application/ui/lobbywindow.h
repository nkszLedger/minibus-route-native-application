#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QJsonObject>
#include <QSqlRecord>
#include <loginform.h>
#include <homeform.h>
#include <memberhomeform.h>
#include <verifymemberform.h>
#include <verifyuserform.h>
#include <portraitcaptureform.h>
#include <fingerprintcaptureform.h>

#include <person.h>

QT_BEGIN_NAMESPACE
namespace Ui { class LobbyWindow; }
QT_END_NAMESPACE

class LobbyWindow : public QMainWindow
{
    Q_OBJECT

public:
    LobbyWindow(QWidget *parent = nullptr);
    ~LobbyWindow();

private slots:
    void close_application();
    void go_to_home_step();
    void go_to_user_verification_step();
    void go_to_member_verification_step();
    void go_to_member_home_step(QJsonObject &person, AdminMode mode);
    void go_to_capture_fingerprint_step(QJsonObject &person, AdminMode mode);
    void go_to_capture_portrait_step(QJsonObject &person, AdminMode mode);

private:
    Ui::LobbyWindow *ui;

    enum {
        LOGIN,
        MEMBERVERIFICATION,
        USERVERIFICATION,
        HOME,
        MEMBERHOME,
        FINGERPRINTCAPTURE,
        PORTRAITCAPTURE
    };

    HomeForm home_form_;
    LoginForm login_form_;
    VerifyMemberForm verify_member_form_;
    VerifyUserForm verify_user_form_;
    MemberHomeForm member_home_form_;
    FingerprintCaptureForm fingerprint_capture_;
    PortraitCaptureForm portrait_capture_;

    /* helper functions */
    void init_step();
    void refresh(QJsonObject &person, AdminMode mode);
};
#endif // LOBBYWINDOW_H
