#ifndef MEMBERHOMEFORM_H
#define MEMBERHOMEFORM_H

#include <QWidget>
#include <QVector>
#include <QSqlField>
#include <QSqlRecord>
#include <QJsonObject>

#include <person.h>

namespace Ui {
class MemberHomeForm;
}

class MemberHomeForm : public QWidget
{
    Q_OBJECT

public:
    explicit MemberHomeForm(QWidget *parent = nullptr);
    ~MemberHomeForm();

    void setPerson(QJsonObject &person, AdminMode mode);

signals:
    void back_button_clicked_signal();
    void fingerprint_capture_clicked_signal(QJsonObject&, AdminMode );
    void portrait_capture_clicked_signal(QJsonObject&, AdminMode);

private slots:
    void on_FingerprintCapturePushButton_clicked();
    void on_PortraitCapturePushButton_clicked();
    void on_BackPushButton_clicked();

private:
    Ui::MemberHomeForm *ui;

    /* Person Details */
    QJsonObject member_;
    AdminMode mode_;
};

#endif // MEMBERHOMEFORM_H
