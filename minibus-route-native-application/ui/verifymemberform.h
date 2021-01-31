#ifndef VERIFYMEMBERFORM_H
#define VERIFYMEMBERFORM_H

#include <QWidget>
#include <api.h>
#include <QJsonObject>

namespace Ui {
class VerifyMemberForm;
}

class VerifyMemberForm : public QWidget
{
    Q_OBJECT

public:
    explicit VerifyMemberForm(QWidget *parent = nullptr);
    ~VerifyMemberForm();

signals:
    void verification_success_signal(QJsonObject&, AdminMode mode);
    void verification_failed_signal();

private slots:
    void on_SearchPushButton_clicked();

    void on_VerificationSuccessful(QJsonObject &person);
    void on_VerificationFailure();
    void on_SystemUserRadioButton_clicked();

    void on_EmployeeRadioButton_clicked();

    void on_MemberRadioButton_clicked();

    void on_MilitaryVeteranRadioButton_clicked();

private:
    Ui::VerifyMemberForm *ui;
    void freeze(bool state);
};

#endif // VERIFYMEMBERFORM_H
