#ifndef FINGERPRINTCAPTUREFORM_H
#define FINGERPRINTCAPTUREFORM_H

#include <QImage>
#include <QMainWindow>
#include <QWidget>
#include <QJsonObject>
#include <QVector>
#include <QSqlField>
#include <QSqlRecord>
#include <api.h>
#include <QProcess>
#include <person.h>

namespace Ui {
class FingerprintCaptureForm;
}

class FingerprintCaptureForm : public QWidget
{
    Q_OBJECT

public:
    explicit FingerprintCaptureForm(QWidget *parent = nullptr);
    ~FingerprintCaptureForm();

    void setPerson(QJsonObject &member, AdminMode mode);

    void setMember(const QString memberDbID);

    void setEmployee(QString employeeDbID);

    void setMilitaryVeteran(QString militaryVeteranDbID);

signals:
    void home_button_clicked_signal(AdminMode mode);

private slots:
    void on_HomePushButton_clicked();

    void on_CapturePushButton_clicked();

    void on_FingerprintSavePushButton_clicked();

    void on_FingerprintRetrievalFailure();

    void on_FingerprintPostSuccessful();
    void on_FingerprintPostFailure();

private:
    Ui::FingerprintCaptureForm *ui;

    QProcess *process_;
    QString program_;

    /* Person Details */
    QString employee_db_id_;
    QString military_veteran_db_id_;
    QString member_db_id_;
    QVector<QSqlRecord> member_;
    AdminMode mode_;


    QImage captured_image_;
    QImage qleft_thumb_image_;
    QImage qright_thumb_image_;
    bool is_left_thumb_captured_;
    bool is_right_thumb_captured_;
    bool is_streaming_;
    bool is_fingerprint_captured_;
    bool stop_streaming_;

    void upload(QString fileName1, QString fileName2);
    void upload(QString fileName);
};

#endif // FINGERPRINTCAPTUREFORM_H
