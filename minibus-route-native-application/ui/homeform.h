#ifndef HOMEFORM_H
#define HOMEFORM_H

#include <QWidget>

namespace Ui {
class HomeForm;
}

class HomeForm : public QWidget
{
    Q_OBJECT

public:
    explicit HomeForm(QWidget *parent = nullptr);
    ~HomeForm();

private slots:
    void on_GoToUserVerificationPushButton_clicked();

    void on_GoToMemberVerificationPushButton_clicked();

signals:
    void user_verification_signal();
    void member_verification_signal();

private:
    Ui::HomeForm *ui;
};

#endif // HOMEFORM_H
