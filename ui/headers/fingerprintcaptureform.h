#ifndef FINGERPRINTCAPTUREFORM_H
#define FINGERPRINTCAPTUREFORM_H

#include <QDialog>

namespace Ui {
class FingerprintCaptureForm;
}

class FingerprintCaptureForm : public QDialog
{
    Q_OBJECT

public:
    explicit FingerprintCaptureForm(QWidget *parent = nullptr);
    ~FingerprintCaptureForm();

private:
    Ui::FingerprintCaptureForm *ui;
};

#endif // FINGERPRINTCAPTUREFORM_H
