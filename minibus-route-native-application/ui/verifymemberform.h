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
    void verification_success_signal(QJsonObject&);
    void verification_failed_signal();

private slots:
    void on_SearchPushButton_clicked();

    void on_VerificationSuccessful(QJsonObject &member);
    void on_VerificationFailure();
private:
    Ui::VerifyMemberForm *ui;
};

#endif // VERIFYMEMBERFORM_H
