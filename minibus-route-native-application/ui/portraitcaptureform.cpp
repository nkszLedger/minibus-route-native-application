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

    /* set flag to update or create to db */
    is_portrait_captured_ = false;

    /* disable to save portrait */
    ui->PortraitSavePushButton->setHidden(true);
    ui->PortraitSaveFrame->setDisabled(true);

    connect(api::instance(), SIGNAL(portrait_details_found(QJsonObject&)),
                                this, SLOT(on_PortraitRetrievalSuccessful(QJsonObject&)));
    connect(api::instance(), SIGNAL(portrait_details_not_found()),
                                this, SLOT(on_PortraitRetrievalFailure()));

    connect(api::instance(), SIGNAL(portrait_post_success()),
                                this, SLOT(on_PortraitPostSuccessful()));
    connect(api::instance(), SIGNAL(portrait_post_failure()),
                                this, SLOT(on_PortraitPostFailure()));
}

PortraitCaptureForm::~PortraitCaptureForm()
{
    /* stop camera */
    if( is_capturing_image_ )
        stopCamera();

    delete ui;
}

void PortraitCaptureForm::on_HomePushButton_clicked()
{
    /* stop camera */
    if( is_capturing_image_ )
        stopCamera();

    if( this->mode_ == ADMINISTER_MEMBER )
    { emit home_button_clicked_signal(ADMINISTER_MEMBER);}
    else{ emit home_button_clicked_signal(ADMINISTER_EMPLOYEE);}
}

void PortraitCaptureForm::on_CapturePushButton_clicked()
{
    if( is_capturing_image_ )
    {
        image_capture_->capture();

        /* enable to save portrait */
        ui->PortraitSavePushButton->setHidden(false);
        ui->PortraitSaveFrame->setDisabled(false);

        /* reset handlers */
        // reset();
    }
    else
    {
        /* set state */
        is_capturing_image_ = true;

        /* start camera */
        startCamera();
    }

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
    /* set state */
    is_capturing_image_ = false;

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
        qDebug() << cameraInfo;

        /* set camera to first available camera */
        QString desc = cameraInfo.description();

        if(desc.startsWith("Logitech")
             || desc.startsWith("FaceCam") )
        {
            setCamera(cameraInfo);
        }
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
    //img.save(current_file_name,"png");
    captured_image_ = img;

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
    /* future use */
}

void PortraitCaptureForm::cleanUp()
{
    QString picturesLocation = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    qDebug() << "PortraitCaptureForm::cleanUp(): " << picturesLocation << endl;
    QDir dir(picturesLocation, {"IMG*.JPG"});
    for(const QString & filename: dir.entryList()){
        dir.remove(filename);
    }
}

void PortraitCaptureForm::on_PortraitSavePushButton_clicked()
{
    /* Long Process!!! Data Transmission */
    QByteArray image_data;
    QBuffer buffer(&image_data);
    buffer.open(QIODevice::WriteOnly);
    captured_image_.save(&buffer, "PNG");

    QString table = "";
    QString id = "";

    if( this->mode_ == ADMINISTER_MEMBER )
    {
        table = "member_portrait";
        id = this->member_db_id_;
    }
    else if( this->mode_ == ADMINISTER_MILITARY_VETERAN )
    {
        table = "military_veteran_portraits";
        id = this->military_veteran_db_id_;
    }
    else
    {
        table = "employee_portrait";
        id = this->employee_db_id_;
    }

    api::instance()->postCapturedPortrait(id,
                                          image_data,
                                          this->mode_,
                                          table,
                                          is_portrait_captured_) ;

}

void PortraitCaptureForm::on_PortraitRetrievalSuccessful(QJsonObject &member)
{
    /*QJsonObject jsonSuccess = member[ "data" ].toObject();
    QString portrait = jsonSuccess.value("portrait").toString();

    QByteArray image_data; // = portrait;

    if( !image_data.isEmpty() )
    {
        is_portrait_captured_ = true;
        QImage image = QImage::fromData(image_data,"PNG");
        captured_image_ = image;
        ui->PortraitCapturedLabel->setPixmap(QPixmap::fromImage(image));
    }
    else
    {
        is_portrait_captured_ = false;
    }*/
}

void PortraitCaptureForm::on_PortraitRetrievalFailure()
{

}

void PortraitCaptureForm::on_PortraitPostSuccessful()
{
    /* Display notification */
    QMessageBox message_box;
    message_box.setWindowOpacity(50);
    message_box.setWindowTitle("Portrait Captured");
    message_box.setStyleSheet("QLabel{ font-weight: plain; font-size: 14px; } \
                                 QPushButton{ width:125px; height:10; font-size: 14px; }");
    message_box.setStandardButtons(QMessageBox::Ok);

    message_box.setIcon(QMessageBox::Information);
    message_box.setText("Portrait Submission Complete");

    message_box.exec();

    /* disable to save portrait */
    ui->PortraitCapturedLabel->clear();
    ui->PortraitSavePushButton->setHidden(true);
    ui->PortraitSaveFrame->setDisabled(true);

    /* remove stored pictures after upload */
    this->cleanUp();
}

void PortraitCaptureForm::on_PortraitPostFailure()
{
    /* Display notification */
    QMessageBox message_box;
    message_box.setWindowOpacity(50);
    message_box.setWindowTitle("Portrait Captured");
    message_box.setStyleSheet("QLabel{ font-weight: plain; font-size: 14px; } \
                                 QPushButton{ width:125px; height:10; font-size: 14px; }");
    message_box.setStandardButtons(QMessageBox::Ok);

    message_box.setIcon(QMessageBox::Warning);
    message_box.setText("Portrait Submission Failed");

    message_box.exec();

    /* remove stored pictures after upload */
    this->cleanUp();
}

void PortraitCaptureForm::setPerson(QJsonObject &member,
                                    AdminMode mode)
{
    /*this->member_ = member;
    this->mode_ = mode;

    QJsonObject jsonSuccess = member[ "data" ].toObject();
    QString id = jsonSuccess.value("id").toString();

    // Retrieve current portrait if any
    api::instance()->getCapturedPortrait(id, mode);*/

}

void PortraitCaptureForm::setMilitaryVeteran(QString militaryVeteranDbID)
{
    this->military_veteran_db_id_ = militaryVeteranDbID;
    this->mode_ = ADMINISTER_MILITARY_VETERAN;

    QByteArray image_data;

    api::instance()->getCapturedPortraitFromDB(this->military_veteran_db_id_,
                                               image_data,
                                               this->mode_,
                                               "military_veteran_portraits");

    if( !image_data.isEmpty() )
    {
        is_portrait_captured_ = true;
        QImage image = QImage::fromData(image_data,"PNG");
        captured_image_ = image;
        ui->PortraitCapturedLabel->setPixmap(QPixmap::fromImage(image));
    }
    else
    {
        qDebug() << "PortraitCaptureForm::setMilitaryVeteran()"
                 << " - Images empty";
        is_portrait_captured_ = false;
    }
}

void PortraitCaptureForm::setEmployee(QString employeeDbID)
{
    this->employee_db_id_ = employeeDbID;
    this->mode_ = ADMINISTER_EMPLOYEE;

    QByteArray image_data;

    api::instance()->getCapturedPortraitFromDB(this->employee_db_id_,
                                               image_data,
                                               this->mode_,
                                               "employee_portrait");

    if( !image_data.isEmpty() )
    {
        is_portrait_captured_ = true;
        QImage image = QImage::fromData(image_data,"PNG");
        captured_image_ = image;
        ui->PortraitCapturedLabel->setPixmap(QPixmap::fromImage(image));
    }
    else
    {
        qDebug() << "PortraitCaptureForm::setEmployee()"
                 << " - Images empty";
        is_portrait_captured_ = false;
    }
}

void PortraitCaptureForm::setMember(QString memberDbID)
{
    this->member_db_id_ = memberDbID;
    this->mode_ = ADMINISTER_MEMBER;

    QByteArray image_data;

    api::instance()->getCapturedPortraitFromDB(this->member_db_id_,
                                               image_data,
                                               this->mode_,
                                               "member_portrait");

    if( !image_data.isEmpty() )
    {
        is_portrait_captured_ = true;
        QImage image = QImage::fromData(image_data,"PNG");
        captured_image_ = image;
        ui->PortraitCapturedLabel->setPixmap(QPixmap::fromImage(image));
    }
    else
    {
        qDebug() << "PortraitCaptureForm::setMember()"
                 << " - Images empty";
        is_portrait_captured_ = false;
    }
}
