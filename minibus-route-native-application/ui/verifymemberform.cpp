#include "verifymemberform.h"
#include "ui_verifymemberform.h"

#include <QMessageBox>
#include <QJsonObject>

VerifyMemberForm::VerifyMemberForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerifyMemberForm)
{
    ui->setupUi(this);

    connect(api::instance(), SIGNAL(member_details_found(QJsonObject&)),
                                this, SLOT(on_VerificationSuccessful(QJsonObject&)));
    connect(api::instance(), SIGNAL(member_details_not_found()),
                                this, SLOT(on_VerificationFailure()));
}

VerifyMemberForm::~VerifyMemberForm()
{
    delete ui;
}

void VerifyMemberForm::on_SearchPushButton_clicked()
{
    QString id = "";
    QString id_type = "";
    QVector<QSqlRecord> result;

    if( ui->VehicleNumberRadioButton->isChecked() )
    {
        id_type = "license_number";
    }
    else if( ui->SAIDRadioButton->isChecked() )
    {
        id_type = "id_number";
    }
    else
    {
        /* Display notification */
        QMessageBox message_box;
        message_box.setWindowOpacity(50);
        message_box.setWindowTitle("Member Verification");
        message_box.setStyleSheet("QLabel{ font-weight: plain; font-size: 14px; } \
                                     QPushButton{ width:125px; height:10; font-size: 14px; }");
        message_box.setStandardButtons(QMessageBox::Ok);

        message_box.setIcon(QMessageBox::Critical);
        message_box.setText("Plase select verification methods");

        message_box.exec();

        return;
    }

    id = ui->MemberIDLineEdit->text();
    api::instance()->isMemberRegistered(id);
}

void VerifyMemberForm::on_VerificationSuccessful(QJsonObject &member)
{
    emit verification_success_signal(member, ADMINISTER_MEMBER);
}

void VerifyMemberForm::on_VerificationFailure()
{
    /* Display notification */
    QMessageBox message_box;
    message_box.setWindowOpacity(50);
    message_box.setWindowTitle("Member Verification");
    message_box.setStyleSheet("QLabel{ font-weight: plain; font-size: 14px; } \
                                 QPushButton{ width:125px; height:10; font-size: 14px; }");
    message_box.setStandardButtons(QMessageBox::Ok);

    message_box.setIcon(QMessageBox::Warning);
    message_box.setText("Member does not exist!");

    message_box.exec();

    emit verification_failed_signal();
}
