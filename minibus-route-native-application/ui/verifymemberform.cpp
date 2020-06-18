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

    /*if( ui->SystemUserRadioButton->isChecked() )
    {

    }
    else if( ui->EmployeeRadioButton->isChecked() )
    {
        id_type = "id_number";
    }*/
    if(ui->MemberRadioButton->isChecked() )
    {
        id = ui->SAIDLineEdit->text();
        api::instance()->isMemberRegistered(id);
    }
    else
    {
        /* Display notification */
        QMessageBox message_box;
        message_box.setWindowOpacity(50);
        message_box.setWindowTitle("Member Verification");
        message_box.setStyleSheet("QLabel{ font-weight: plain; \
                                            font-size: 14px; } \
                                     QPushButton{ width:125px; \
                                        height:10; font-size: 14px; }");

        message_box.setStandardButtons(QMessageBox::Ok);
        message_box.setIcon(QMessageBox::Critical);
        message_box.setText("Feature coming soon. Plase select MEMBER verification method");

        message_box.exec();

        return;
    }
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
