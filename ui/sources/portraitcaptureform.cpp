#include "ui\headers\portraitcaptureform.h"
#include "ui_portraitcaptureform.h"

#include <QCameraInfo>

Q_DECLARE_METATYPE(QCameraInfo)

PortraitCaptureForm::PortraitCaptureForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PortraitCaptureForm)
{
    ui->setupUi(this);

    /* start live stream
    startCamera(); */

}

PortraitCaptureForm::~PortraitCaptureForm()
{
    delete ui;
}

void PortraitCaptureForm::on_CapturePushButton_clicked()
{
    /* stop camera */
    camera_->stop();

    /* reset handlers */
    // reset();
}
void PortraitCaptureForm::startCamera()
{
    /* Check Devices */
    scanDevices();

    /* start camera */
    camera_->start();
}
void PortraitCaptureForm::reset()
{
    /* reset */
    delete camera_;
    delete image_capture_;

    camera_ = nullptr;
    image_capture_ = nullptr;
}

int PortraitCaptureForm::scanDevices()
{
    int counter = 0;

    /* list Camera devices in combobox */
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    qDebug() << "PortraitCaptureForm::scanDevices() - Listed Cameras";

    foreach (const QCameraInfo &cameraInfo, cameras)
    {
        qDebug() << "PortraitCaptureForm::scanDevices() - About to set Cameras";

        /* set camera to first available camera */
        if(counter==0)
            setCamera(cameraInfo);

        counter++;
    }

    qDebug() << "PortraitCaptureForm::scanDevices() - Set Cameras";

    return counter;
}

void PortraitCaptureForm::setCamera(const QCameraInfo &cameraInfo)
{
    /* delete previous camera and its related settings */
//    if(camera_ && image_capture_)
//        reset();

    /* declare new camera */
    camera_ = new QCamera(cameraInfo);
    qDebug() << "setCamera: " << cameraInfo.description();

    /* declare new image capture */

    image_capture_ = new QCameraImageCapture(camera_);
    connect(image_capture_, SIGNAL(imageCaptured(int,QImage)), this, SLOT(processCapturedImage(int,QImage)));
    connect(image_capture_, SIGNAL(imageSaved(int,QString)),   this, SLOT(deleteTempDir(int,QString)));

    /* set viewfinder (streaming label) */
    camera_->setViewfinder(ui->StreamingWindow_viewfinder);

    /* check if camera is supported */
    updateCaptureMode();

}

void PortraitCaptureForm::updateCaptureMode()
{
    QCamera::CaptureModes captureMode = QCamera::CaptureStillImage;

    if (camera_->isCaptureModeSupported(captureMode))
        camera_->setCaptureMode(captureMode);
}

