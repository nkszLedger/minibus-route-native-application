#include "verifymemberform.h"
#include "ui_verifymemberform.h"

VerifyMemberForm::VerifyMemberForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VerifyMemberForm)
{
    ui->setupUi(this);
}

VerifyMemberForm::~VerifyMemberForm()
{
    delete ui;
}

void VerifyMemberForm::on_FingerprintCapturePushButton_clicked()
{
    fingerprint_window_.show();
}

void VerifyMemberForm::on_PortraitCapturePushButton_clicked()
{
    portrait_window_.show();
}
