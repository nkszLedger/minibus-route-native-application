#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <api.h>

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

signals:
    void close_application_signal();
    void login_success_signal();
    void login_failed_signal();

private slots:
    void on_LoginPushButton_clicked();
    void on_CancelPushButton_clicked();
    void on_LoginFailure();
    void on_LoginSuccess();

private:
    Ui::LoginForm *ui;
};

#endif // LOGINFORM_H
