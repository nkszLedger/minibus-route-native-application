#include "portraitcaptureform.h"
#include "ui_portraitcaptureform.h"

#include <QDateTime>
#include <QMessageBox>
#include <QCameraInfo>

PortraitCaptureForm::PortraitCaptureForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PortraitCaptureForm)
{
    ui->setupUi(this);

    /* initialize */
    is_capturing_image_ = false;

    /* start camera */
    startCamera();

    /* disable to save portrait */
    ui->PortraitSavePushButton->setHidden(true);
    ui->PortraitSaveFrame->setDisabled(true);
}

PortraitCaptureForm::~PortraitCaptureForm()
{
    /* stop camera */
    stopCamera();

    delete ui;
}

void PortraitCaptureForm::on_HomePushButton_clicked()
{
    /* stop camera */
    stopCamera();

    int step = 4;
    emit home_button_clicked_signal(step);
}

void PortraitCaptureForm::on_CapturePushButton_clicked()
{
    is_capturing_image_ = true;
    image_capture_->capture();

    /* enable to save portrait */
    ui->PortraitSavePushButton->setHidden(false);
    ui->PortraitSaveFrame->setDisabled(false);

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

void PortraitCaptureForm::stopCamera()
{
    /* start camera */
    camera_->stop();

    /* reset - crashes here?? */
    //reset();
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

void PortraitCaptureForm::processCapturedImage(int requestId, const QImage& img)
{
    qDebug() << "HERE";

    // unused varaible
    Q_UNUSED(requestId);

    QString fileName = QString("%1.png")
                                .arg(QDateTime::currentDateTime().toString("yyMMdd_hh-mm-ss"));

    QString current_file_name = fileName;
    img.save(current_file_name,"png");

    ui->PortraitCapturedLabel->setPixmap(QPixmap::fromImage(img));
}

void PortraitCaptureForm::savePrints(QString directory)
{
    QString fileName = QString("%1.png")
                                .arg(QDateTime::currentDateTime().toString("yyMMdd_hh-mm-ss"));

    QString current_file_name = fileName;
}

void PortraitCaptureForm::deleteTempDir(int id, QString path)
{

}

void PortraitCaptureForm::on_PortraitSavePushButton_clicked()
{
    QMessageBox message_box;
    message_box.setWindowOpacity(50);
    message_box.setWindowTitle("Portrait Captured");
    message_box.setStyleSheet("QLabel{ font-weight: plain; font-size: 14px; } \
                                 QPushButton{ width:125px; height:10; font-size: 14px; }");
    message_box.setStandardButtons(QMessageBox::Ok);
    message_box.setIcon(QMessageBox::Information);
    message_box.setText("Portrait sent to db");

    message_box.exec();

    /* disable to save portrait */
    ui->PortraitCapturedLabel->clear();
    ui->PortraitSavePushButton->setHidden(true);
    ui->PortraitSaveFrame->setDisabled(true);
}
