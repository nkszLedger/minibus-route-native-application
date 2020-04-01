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

void PortraitCaptureForm::on_HomePushButton_clicked()
{
    emit home_button_clicked_signal();
}
