#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <loginform.h>
#include <memberhomeform.h>
#include <verifymemberform.h>

QT_BEGIN_NAMESPACE
namespace Ui { class LobbyWindow; }
QT_END_NAMESPACE

class LobbyWindow : public QMainWindow
{
    Q_OBJECT

public:
    LobbyWindow(QWidget *parent = nullptr);
    ~LobbyWindow();

private:
    Ui::LobbyWindow *ui;

    enum {
        LOGIN,
        VERIFICATION,
        HOME
    };

    LoginForm login_form_;
    VerifyMemberForm verify_member_form_;
    MemberHomeForm member_home_form_;
};
#endif // LOBBYWINDOW_H
