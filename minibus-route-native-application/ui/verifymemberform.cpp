#include "verifymemberform.h"
#include "ui_verifymemberform.h"

#include <QMessageBox>
#include <QJsonObject>

VerifyMemberForm::VerifyMemberForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerifyMemberForm)
{
    ui->setupUi(this);

    /* member details status signals */
    connect(api::instance(), SIGNAL(member_details_found(QJsonObject&)),
                                this, SLOT(on_VerificationSuccessful(QJsonObject&)));
    connect(api::instance(), SIGNAL(member_details_not_found()),
                                this, SLOT(on_VerificationFailure()));

    /* employee details status signals */
    connect(api::instance(), SIGNAL(employee_details_found(QJsonObject&)),
                                this, SLOT(on_VerificationSuccessful(QJsonObject&)));
    connect(api::instance(), SIGNAL(employee_details_not_found()),
                                this, SLOT(on_VerificationFailure()));
}

VerifyMemberForm::~VerifyMemberForm()
{
    delete ui;
}

void VerifyMemberForm::on_SearchPushButton_clicked()
{
    QString id = ui->SAIDLineEdit->text();
    QString id_type = "";
    QVector<QSqlRecord> result;

    /* freeze */
    this->freeze(true);

    if( id.isEmpty() )
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
        message_box.setText("Please enter ID number");

        message_box.exec();

        /* freeze */
        this->freeze(false);
        return;
    }

    /*if( ui->SystemUserRadioButton->isChecked() )
    {

    }*/
    if( ui->EmployeeRadioButton->isChecked() )
    {
        api::instance()->isEmployeeRegistered(id);
    }
    else if(ui->MemberRadioButton->isChecked() )
    {
        api::instance()->isMemberRegistered(id);
    }
    else if(ui->MilitaryVeteranRadioButton->isChecked() )
    {
        api::instance()->isMilitaryVeteranRegistered(id);
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
        message_box.setText("User Capturing coming soon. Plase select verification method");

        message_box.exec();
    }

    /* unfreeze */
    this->freeze(false);

}

void VerifyMemberForm::freeze(bool state)
{
    ui->MemberRadioButton->setDisabled(state);
    ui->EmployeeRadioButton->setDisabled(state);
    ui->SystemUserRadioButton->setDisabled(state);
    ui->MilitaryVeteranRadioButton->setDisabled(state);
}

void VerifyMemberForm::on_VerificationSuccessful(QJsonObject &person)
{
    /* unfreeze */
    this->freeze(false);

    if( ui->EmployeeRadioButton->isChecked() )
    {
        emit verification_success_signal(person, ADMINISTER_EMPLOYEE);
    }
    else if(ui->MilitaryVeteranRadioButton->isChecked() )
    {
        emit verification_success_signal(person, ADMINISTER_MILITARY_VETERAN);
    }
    else if(ui->MemberRadioButton->isChecked() )
    {
        emit verification_success_signal(person, ADMINISTER_MEMBER);
    }
}

void VerifyMemberForm::on_VerificationFailure()
{
    /* Display notification */
    QMessageBox message_box;
    message_box.setWindowOpacity(50);
    message_box.setWindowTitle("Subject Verification");
    message_box.setStyleSheet("QLabel{ font-weight: plain; font-size: 14px; } \
                                 QPushButton{ width:125px; height:10; font-size: 14px; }");
    message_box.setStandardButtons(QMessageBox::Ok);

    message_box.setIcon(QMessageBox::Warning);

    if( ui->EmployeeRadioButton->isChecked() )
    {   message_box.setText("Minibus Employee does not exist!");  }
    else if( ui->MilitaryVeteranRadioButton->isChecked() )
    {   message_box.setText("Military Veteran does not exist!");  }
    else{  message_box.setText("Member does not exist!"); }

    message_box.exec();

    /* unfreeze */
    this->freeze(false);

    emit verification_failed_signal();
}

void VerifyMemberForm::on_SystemUserRadioButton_clicked()
{
    ui->TitleLabel->setText("System User Verification");
}

void VerifyMemberForm::on_EmployeeRadioButton_clicked()
{
    ui->TitleLabel->setText("Minibus Employee Verification");
}

void VerifyMemberForm::on_MemberRadioButton_clicked()
{
    ui->TitleLabel->setText("Minibus Member Verification");
}


void VerifyMemberForm::on_MilitaryVeteranRadioButton_clicked()
{
    ui->TitleLabel->setText("Military Veteran Verification");
}
