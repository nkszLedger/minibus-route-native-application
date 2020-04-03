#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QSqlRecord>
#include <loginform.h>
#include <memberhomeform.h>
#include <verifymemberform.h>
#include <portraitcaptureform.h>
#include <fingerprintcaptureform.h>

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
    void go_to_verification_step();
    void go_to_member_home_step(QVector<QSqlRecord> &member);
    void go_to_capture_fingerprint_step(QVector<QSqlRecord> &member);
    void go_to_capture_portrait_step(QVector<QSqlRecord> &member);

private:
    Ui::LobbyWindow *ui;

    enum {
        LOGIN,
        VERIFICATION,
        HOME,
        FINGERPRINTCAPTURE,
        PORTRAITCAPTURE
    };

    LoginForm login_form_;
    VerifyMemberForm verify_member_form_;
    MemberHomeForm member_home_form_;
    FingerprintCaptureForm fingerprint_capture_;
    PortraitCaptureForm portrait_capture_;

    /* helper functions */
    void init_step();
};
#endif // LOBBYWINDOW_H
