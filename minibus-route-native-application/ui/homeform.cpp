#include "ui\homeform.h"
#include "ui_homeform.h"

HomeForm::HomeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeForm)
{
    ui->setupUi(this);
}

HomeForm::~HomeForm()
{
    delete ui;
}

void HomeForm::on_GoToUserVerificationPushButton_clicked()
{
    emit user_verification_signal();
}

void HomeForm::on_GoToMemberVerificationPushButton_clicked()
{
    emit member_verification_signal();
}
