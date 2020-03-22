#include "ui\headers\loginform.h"
#include "ui_loginform.h"

LoginForm::LoginForm(QWidget *parent) :
  QDialog(parent),
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
    //emit login_success();
    this->close();
}
