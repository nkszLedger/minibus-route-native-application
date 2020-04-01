#include "loginform.h"
#include "ui_loginform.h"

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
    emit login_success_signal();
}

void LoginForm::on_CancelPushButton_clicked()
{
    emit close_application_signal();
}
