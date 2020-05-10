#include "ui\verifyuserform.h"
#include "ui_verifyuserform.h"

VerifyUserForm::VerifyUserForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerifyUserForm)
{
    ui->setupUi(this);
}

VerifyUserForm::~VerifyUserForm()
{
    delete ui;
}

void VerifyUserForm::on_SearchPushButton_clicked()
{
    emit verification_success_signal();
}
