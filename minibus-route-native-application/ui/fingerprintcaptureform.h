#ifndef FINGERPRINTCAPTUREFORM_H
#define FINGERPRINTCAPTUREFORM_H

#include <QWidget>

namespace Ui {
class FingerprintCaptureForm;
}

class FingerprintCaptureForm : public QWidget
{
    Q_OBJECT

public:
    explicit FingerprintCaptureForm(QWidget *parent = nullptr);
    ~FingerprintCaptureForm();

signals:
    void home_button_clicked_signal();

private slots:
    void on_HomePushButton_clicked();

private:
    Ui::FingerprintCaptureForm *ui;
};

#endif // FINGERPRINTCAPTUREFORM_H
