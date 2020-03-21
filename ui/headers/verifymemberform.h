#ifndef VERIFYMEMBERFORM_H
#define VERIFYMEMBERFORM_H

#include <QDialog>

#include "fingerprintcaptureform.h"
#include "portraitcaptureform.h"

namespace Ui {
class VerifyMemberForm;
}

class VerifyMemberForm : public QDialog
{
    Q_OBJECT

public:
    explicit VerifyMemberForm(QWidget *parent = nullptr);
    ~VerifyMemberForm();

private slots:
    void on_FingerprintCapturePushButton_clicked();

    void on_PortraitCapturePushButton_clicked();

private:
    Ui::VerifyMemberForm *ui;

    FingerprintCaptureForm fingerprint_window_;
    PortraitCaptureForm portrait_window_;
};

#endif // VERIFYMEMBERFORM_H
