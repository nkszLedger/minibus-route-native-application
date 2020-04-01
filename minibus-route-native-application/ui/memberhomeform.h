#ifndef MEMBERHOMEFORM_H
#define MEMBERHOMEFORM_H

#include <QWidget>

namespace Ui {
class MemberHomeForm;
}

class MemberHomeForm : public QWidget
{
    Q_OBJECT

public:
    explicit MemberHomeForm(QWidget *parent = nullptr);
    ~MemberHomeForm();

signals:
    void back_button_clicked_signal();
    void fingerprint_capture_clicked_signal(QString member_id);
    void portrait_capture_clicked_signal(QString member_id);

private slots:
    void on_FingerprintCapturePushButton_clicked();
    void on_PortraitCapturePushButton_clicked();
    void on_BackPushButton_clicked();

private:
    Ui::MemberHomeForm *ui;
};

#endif // MEMBERHOMEFORM_H
