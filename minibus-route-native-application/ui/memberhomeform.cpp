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
    emit fingerprint_capture_clicked_signal(member_, mode_);
}


void MemberHomeForm::on_PortraitCapturePushButton_clicked()
{
    emit portrait_capture_clicked_signal(member_, mode_);
}

void MemberHomeForm::on_BackPushButton_clicked()
{
    emit back_button_clicked_signal();
}

void MemberHomeForm::setPerson(QJsonObject &person, AdminMode mode)
{
    this->member_ = person;
    this->mode_ = mode;

    /* Populate lobby with Person details */
    QJsonObject jsonSuccess = person[ "data" ].toObject();
    QString id_number  = jsonSuccess.value("id_number").toString( );
    QString license_number = jsonSuccess.value("email").toString();
    QString name  = jsonSuccess.value("name").toString();
    QString surname  = jsonSuccess.value("surname").toString();
    qDebug() << "name: " << name;

    ui->MemberFullNameLineEdit->setText( name +" "+ surname );
    ui->VehicleRegNoLineEdit->setText( license_number );
    ui->MemberSAIDLineEdit->setText( id_number );

    if( mode == ADMINISTER_MEMBER)
    {
        QString id_number  = jsonSuccess.value("id_number").toString( );
        QString license_number = jsonSuccess.value("email").toString();
        QString name  = jsonSuccess.value("name").toString();
        QString surname  = jsonSuccess.value("surname").toString();

        ui->MemberFullNameLineEdit->setText( name +" "+ surname );
        ui->VehicleRegNoLineEdit->setText( license_number );
        ui->MemberSAIDLineEdit->setText( id_number );
    }
    else
    {
        QString email = jsonSuccess.value("email").toString();
        /*QString id_number  = jsonSuccess.value("id_number").toString( );
        QString name  = jsonSuccess.value("name").toString();
        QString surname  = jsonSuccess.value("surname").toString();*/

        //ui->MemberFullNameLineEdit->setText( name +" "+ surname );
        ui->VehicleRegNoLineEdit->setText( email );
        //ui->MemberSAIDLineEdit->setText( id_number );
    }

    ui->MemberFullNameLineEdit->setDisabled(true);
    ui->VehicleRegNoLineEdit->setDisabled(true);
    ui->MemberSAIDLineEdit->setDisabled(true);

}
