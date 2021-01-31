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
    QString email = jsonSuccess.value("email").toString();

    qDebug() << "name: " << name;

    if( mode == ADMINISTER_MEMBER )
    {
        ui->CaptureTitleLabel->setText("Member Capture");
        ui->DetailsTitleLabel->setText("Member Details");
        ui->MemberSAIDLabel->setText("Member SA ID");
        ui->MemberFullNameLabel->setText("Member Fullname");
        ui->VehicleRegNoLabel->setText("Member Email");
        ui->VehicleRegNoLineEdit->setText( license_number );
        ui->MemberSAIDLineEdit->setText( id_number );
    }
    else if( mode == ADMINISTER_MILITARY_VETERAN )
    {
        ui->CaptureTitleLabel->setText("Military Veteran Capture");
        ui->DetailsTitleLabel->setText("MKV Details");
        ui->MemberSAIDLabel->setText("MKV SA ID");
        ui->MemberFullNameLabel->setText("MKV Fullname");
        ui->VehicleRegNoLabel->setText("MKV Email");
        ui->VehicleRegNoLineEdit->setText( email );
    }
    else
    {
        ui->CaptureTitleLabel->setText("Employee Capture");
        ui->DetailsTitleLabel->setText("Employee Details");
        ui->MemberSAIDLabel->setText("Employee SA ID");
        ui->MemberFullNameLabel->setText("Employee Fullname");
        ui->VehicleRegNoLabel->setText("Employee Email");
        ui->VehicleRegNoLineEdit->setText( email );
    }

    ui->MemberSAIDLineEdit->setText( id_number );
    ui->MemberFullNameLineEdit->setText( name +" "+ surname );

    ui->MemberFullNameLineEdit->setDisabled(true);
    ui->VehicleRegNoLineEdit->setDisabled(true);
    ui->MemberSAIDLineEdit->setDisabled(true);

}
