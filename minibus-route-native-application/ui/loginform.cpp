#include "loginform.h"
#include "ui_loginform.h"

#include <QMessageBox>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::on_LoginPushButton_clicked()
{
    QString username = ui->UsernameLineEdit->text();
    QString password = ui->PasswordLineEdit->text();

    api service;
    if( service.authenticateUser( username, password ) )
    {
       emit login_success_signal();
    }
    else
    {
        /* Display notification */
        QMessageBox message_box;
        message_box.setWindowOpacity(50);
        message_box.setWindowTitle("User Login");
        message_box.setStyleSheet("QLabel{ font-weight: plain; font-size: 14px; } \
                                     QPushButton{ width:125px; height:10; font-size: 14px; }");
        message_box.setStandardButtons(QMessageBox::Ok);

        message_box.setIcon(QMessageBox::Warning);
        message_box.setText("Username/Password Incorrect");

        message_box.exec();

    }


}

void LoginForm::on_CancelPushButton_clicked()
{
    emit close_application_signal();
}
