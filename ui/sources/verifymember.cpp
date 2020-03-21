#include "verifymember.h"
#include "ui_verifymember.h"

VerifyMember::VerifyMember(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VerifyMember)
{
    ui->setupUi(this);
}

VerifyMember::~VerifyMember()
{
    delete ui;
}
