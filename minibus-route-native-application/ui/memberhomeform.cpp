#include "memberhomeform.h"
#include "ui_memberhomeform.h"

MemberHomeForm::MemberHomeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MemberHomeForm)
{
    ui->setupUi(this);
}

MemberHomeForm::~MemberHomeForm()
{
    delete ui;
}

void MemberHomeForm::on_FingerprintCapturePushButton_clicked()
{
    emit fingerprint_capture_clicked_signal("123");
}


void MemberHomeForm::on_PortraitCapturePushButton_clicked()
{
    emit portrait_capture_clicked_signal("123");
}

void MemberHomeForm::on_BackPushButton_clicked()
{
    emit back_button_clicked_signal();
}
