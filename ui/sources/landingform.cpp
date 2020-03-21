#include "ui\headers\landingform.h"
#include "ui_landingform.h"

LandingForm::LandingForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LandingForm)
{
    ui->setupUi(this);
}

LandingForm::~LandingForm()
{
    delete ui;
}
