#include "memberhomeform.h"
#include "ui_memberhomeform.h"
#include <QDebug>


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
    emit fingerprint_capture_clicked_signal(member_);
}


void MemberHomeForm::on_PortraitCapturePushButton_clicked()
{
    emit portrait_capture_clicked_signal(member_);
}

void MemberHomeForm::on_BackPushButton_clicked()
{
    emit back_button_clicked_signal();
}

void MemberHomeForm::setMember(QJsonObject &member)
{
    this->member_ = member;

    /* Populate lobby with Person details */
    QJsonObject jsonSuccess = member[ "data" ].toObject();

    QString id_number  = jsonSuccess.value("id_number").toString( );
    QString license_number  = jsonSuccess.value("email").toString();
    QString name  = jsonSuccess.value("name").toString();
    QString surname  = jsonSuccess.value("surname").toString();

    qDebug() << "id: " << id_number;

    ui->MemberFullNameLineEdit->setText( name +" "+ surname );
    ui->VehicleRegNoLineEdit->setText( license_number );
    ui->MemberSAIDLineEdit->setText( id_number );

    ui->MemberFullNameLineEdit->setDisabled(true);
    ui->VehicleRegNoLineEdit->setDisabled(true);
    ui->MemberSAIDLineEdit->setDisabled(true);

}
