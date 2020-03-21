#include "ui\headers\fingerprintcaptureform.h"
#include "ui_fingerprintcaptureform.h"

FingerprintCaptureForm::FingerprintCaptureForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FingerprintCaptureForm)
{
    ui->setupUi(this);
}

FingerprintCaptureForm::~FingerprintCaptureForm()
{
    delete ui;
}
