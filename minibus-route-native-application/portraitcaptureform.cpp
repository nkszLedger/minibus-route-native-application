#include "portraitcaptureform.h"
#include "ui_portraitcaptureform.h"

PortraitCaptureForm::PortraitCaptureForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PortraitCaptureForm)
{
    ui->setupUi(this);
}

PortraitCaptureForm::~PortraitCaptureForm()
{
    delete ui;
}
