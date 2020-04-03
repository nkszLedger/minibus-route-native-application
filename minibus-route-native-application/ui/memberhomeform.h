#ifndef MEMBERHOMEFORM_H
#define MEMBERHOMEFORM_H

#include <QWidget>
#include <QVector>
#include <QSqlField>
#include <QSqlRecord>

namespace Ui {
class MemberHomeForm;
}

class MemberHomeForm : public QWidget
{
    Q_OBJECT

public:
    explicit MemberHomeForm(QWidget *parent = nullptr);
    ~MemberHomeForm();

    void setMember(const QVector<QSqlRecord> &member);

signals:
    void back_button_clicked_signal();
    void fingerprint_capture_clicked_signal(QVector<QSqlRecord> &member);
    void portrait_capture_clicked_signal(QVector<QSqlRecord> &member);

private slots:
    void on_FingerprintCapturePushButton_clicked();
    void on_PortraitCapturePushButton_clicked();
    void on_BackPushButton_clicked();

private:
    Ui::MemberHomeForm *ui;

    /* Member Details */
    QVector<QSqlRecord> member_;
};

#endif // MEMBERHOMEFORM_H
