#ifndef PORTRAITCAPTUREFORM_H
#define PORTRAITCAPTUREFORM_H

#include <QDialog>
#include <QCamera>
#include <QCameraImageCapture>

namespace Ui {
class PortraitCaptureForm;
}

class PortraitCaptureForm : public QDialog
{
    Q_OBJECT

public:
    explicit PortraitCaptureForm(QWidget *parent = nullptr);
    ~PortraitCaptureForm();


private slots:
    void on_CapturePushButton_clicked();

private:
    Ui::PortraitCaptureForm *ui;

    /* QCamera */
    QCamera *camera_;
    QCameraImageCapture *image_capture_;

    /* helper functions */
    void startCamera();
    int scanDevices();
    void setCamera(const QCameraInfo &cameraInfo);
    void updateCaptureMode();
    void reset();
};

#endif // PORTRAITCAPTUREFORM_H
