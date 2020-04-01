#include "fingerprintcaptureform.h"
#include "ui_fingerprintcaptureform.h"

FingerprintCaptureForm::FingerprintCaptureForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FingerprintCaptureForm)
{
    ui->setupUi(this);
}

FingerprintCaptureForm::~FingerprintCaptureForm()
{
    delete ui;
}

void FingerprintCaptureForm::on_HomePushButton_clicked()
{
    emit home_button_clicked_signal();
}
