#ifndef VERIFYMEMBERFORM_H
#define VERIFYMEMBERFORM_H

#include <QWidget>

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
    void verification_success_signal();
    void verification_failed_signal();

private slots:
    void on_SearchPushButton_clicked();

private:
    Ui::VerifyMemberForm *ui;
};

#endif // VERIFYMEMBERFORM_H
