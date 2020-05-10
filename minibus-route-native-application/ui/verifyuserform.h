#ifndef VERIFYUSERFORM_H
#define VERIFYUSERFORM_H

#include <QWidget>

namespace Ui {
class VerifyUserForm;
}

class VerifyUserForm : public QWidget
{
    Q_OBJECT

public:
    explicit VerifyUserForm(QWidget *parent = nullptr);
    ~VerifyUserForm();

private slots:
    void on_SearchPushButton_clicked();

signals:
    void verification_success_signal();
    void verification_failed_signal();

private:
    Ui::VerifyUserForm *ui;
};

#endif // VERIFYUSERFORM_H
