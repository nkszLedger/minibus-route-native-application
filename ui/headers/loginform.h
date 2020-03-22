#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>

namespace Ui {
class LoginForm;
}

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

//signals:
//    void login_success();
//    void login_failed();

private slots:
    void on_LoginPushButton_clicked();

private:
    Ui::LoginForm *ui;
};

#endif // LOGINFORM_H
