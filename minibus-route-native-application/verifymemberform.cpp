#include "verifymemberform.h"
#include "ui_verifymemberform.h"

VerifyMemberForm::VerifyMemberForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerifyMemberForm)
{
    ui->setupUi(this);
}

VerifyMemberForm::~VerifyMemberForm()
{
    delete ui;
}
