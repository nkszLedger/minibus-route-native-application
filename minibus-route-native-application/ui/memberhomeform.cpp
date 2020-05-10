#include "memberhomeform.h"
#include "ui_memberhomeform.h"


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
    emit fingerprint_capture_clicked_signal(person_);
}


void MemberHomeForm::on_PortraitCapturePushButton_clicked()
{
    emit portrait_capture_clicked_signal(person_);
}

void MemberHomeForm::on_BackPushButton_clicked()
{
    emit back_button_clicked_signal();
}

void MemberHomeForm::setPerson(Person *person)
{
    person_ = new Person(person);

    /* Populate lobby with Person details */
    QSqlRecord record = person_->personData().at(0);

    if( person_->personType() == REGISTERED_MEMBER )
    {
        QString id_number  = record.field("id_number").value().toString();
        QString license_number  = record.field("license_number").value().toString();
        QString name  = record.field("name").value().toString();
        QString surname  = record.field("surname").value().toString();

        ui->MemberFullNameLineEdit->setText( name +" "+ surname );
        ui->VehicleRegNoLineEdit->setText( license_number );
        ui->MemberSAIDLineEdit->setText( id_number );

        ui->MemberFullNameLineEdit->setDisabled(true);
        ui->VehicleRegNoLineEdit->setDisabled(true);
        ui->MemberSAIDLineEdit->setDisabled(true);
    }
    else
    {

    }
}
