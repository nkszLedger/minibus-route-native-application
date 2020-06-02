#include "fingerprintcaptureform.h"
#include "ui_fingerprintcaptureform.h"

#include <QMessageBox>

FingerprintCaptureForm::FingerprintCaptureForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FingerprintCaptureForm)
{
    ui->setupUi(this);

    /* initialize fields */
    stored_image_counter_ = 0;
    c_buffer_size_        = 500;
    current_image_width_  = c_buffer_size_;
    current_image_height_ = c_buffer_size_;
    current_image_        = new (nothrow) unsigned char[current_image_width_*current_image_height_];

    /* setup scanner */
    scanner_ = new FutronicScanner();

    /* set is streaming control status */
    is_streaming_ = false;
    stop_streaming_ = false;

    /* set flag to update or create to db */
    is_fingerprint_captured_ = false;

    /* set flags to indicate fingers captured */
    is_left_thumb_captured_ = false;
    is_right_thumb_captured_ = false;

    /* connect statements */
    connect(scanner_, SIGNAL(link(int)), this, SLOT(on_update_screen_during_streaming(int)));
    connect(scanner_, SIGNAL(image_link(QByteArray,int)), this, SLOT(on_image_receive(QByteArray,int)));

    connect(api::instance(), SIGNAL(member_fingerprint_details_found(QJsonObject&)),
                                this, SLOT(on_FingerprintRetrievalSuccessful(QJsonObject&)));
    connect(api::instance(), SIGNAL(member_fingerprint_details_not_found()),
                                this, SLOT(on_FingerprintRetrievalFailure()));
    connect(api::instance(), SIGNAL(member_fingerprints_post_success()),
                                this, SLOT(on_FingerprintPostSuccessful()));
    connect(api::instance(), SIGNAL(member_fingerprints_post_failure()),
                                this, SLOT(on_FingerprintPostlFailure()));
}

void FingerprintCaptureForm::on_update_screen_during_streaming(int flag)
{
  /* clear display */
  ui->FingerprintCaptureLabel->clear();

  if(flag == -1)
  {
      ui->FingerprintCaptureLabel->setText("Please Place Finger on Scanner");
      /*ui->quality_progressBar->setValue(0);*/
  }
  else
  {
      /* get image of stored struct */
      QImage image(current_image_,\
                   current_image_width_,\
                   current_image_height_,\
                   QImage::Format_Indexed8);

      /* display image of stored struct */
      ui->FingerprintCaptureLabel->setPixmap(QPixmap::fromImage(image));

      /* display quality of stored struct */
      /*ui->quality_progressBar->setValue(int(flag));*/

      if(!is_streaming_)
      {
        current_image_width_  = c_buffer_size_;
        current_image_height_ = c_buffer_size_;
      }
  }
  // update interface
  ui->FingerprintCaptureLabel->repaint();
  QCoreApplication::processEvents();
}

FingerprintCaptureForm::~FingerprintCaptureForm()
{
    delete ui;
}

void FingerprintCaptureForm::on_HomePushButton_clicked()
{
    emit home_button_clicked_signal(member_);
}

void FingerprintCaptureForm::on_CapturePushButton_clicked()
{
    ui->FingerprintCaptureLabel->clear();

    if( !is_streaming_ )
    {
        is_streaming_ = true;
        stop_streaming_ = false;
        scanner_->setControl(stop_streaming_);

        /* open scanner */
        scanner_->open(0);

        /* start stream */
        scanner_->stream(current_image_, current_image_width_, current_image_height_, stop_streaming_ );
    }
    else
    {
        /* set stop streaming control */
        stop_streaming_ = true;
        scanner_->setControl(stop_streaming_);

        /* set is streaming */
        is_streaming_ = false;

        /* update threads */
        QCoreApplication::processEvents();
        /* close scanner */
        scanner_->close();

        /* display image */
        ui->FingerprintCapturedLabel->setPixmap(QPixmap::fromImage(captured_image_));
        ui->FingerprintSavePushButton->setHidden(false);

        /* display mini images indicating sides */
        if( ui->LeftRadioButton->isChecked() )
        {
            is_left_thumb_captured_ = true;
            left_thumb_image_ = captured_image_;
            ui->ImageOnelabel->setPixmap(QPixmap::fromImage(captured_image_));
        }
        else if( ui->RightRadioButton->isChecked() )
        {
            is_right_thumb_captured_ = true;
            right_thumb_image_ = captured_image_;
            ui->ImageTwolabel->setPixmap(QPixmap::fromImage(captured_image_));
        }
        /* reset variables */
        current_image_width_  = c_buffer_size_;
        current_image_height_ = c_buffer_size_;
    }
    return;
}

void FingerprintCaptureForm::on_image_receive(QByteArray Image, int quality)
{
    /* check quality against threshold */
    if(quality >= 50)
    {
        /* create new finger Struct */
        fingerData current_finger;
        current_finger.initialiseMemory(current_image_width_*current_image_height_);

        /* populate values to struct */
        current_finger.setFingerDataValues((unsigned char*)Image.data(),\
                                            quality,
                                            current_image_width_, \
                                            current_image_height_);

        /* save image captured */
        QImage image(current_finger.image,\
                     current_finger.image_width,\
                     current_finger.image_height,\
                     QImage::Format_Indexed8);

        captured_image_ = image;
    }

    /* test update stream from here */
    on_update_screen_during_streaming(quality);
}

void FingerprintCaptureForm::on_FingerprintSavePushButton_clicked()
{
    /* Long Process!!! Data Transmission */
    QByteArray left_thumb_image_data;
    QBuffer lbuffer(&left_thumb_image_data);
    lbuffer.open(QIODevice::WriteOnly);
    left_thumb_image_.save(&lbuffer, "PNG");

    QByteArray right_thumb_image_data;
    QBuffer rbuffer(&right_thumb_image_data);
    rbuffer.open(QIODevice::WriteOnly);
    right_thumb_image_.save(&rbuffer, "PNG");

    QJsonObject jsonSuccess = member_[ "data" ].toObject();
    QString id = jsonSuccess.value("id").toString();

    api::instance()->postCapturedFingerprint(id, left_thumb_image_data,
                                             right_thumb_image_data,
                                             this->mode_, is_fingerprint_captured_ );

    /* clear captured content */
    ui->FingerprintCapturedLabel->clear();
    is_left_thumb_captured_ = false;
    is_right_thumb_captured_ = false;
}

void FingerprintCaptureForm::on_FingerprintRetrievalSuccessful(QJsonObject &member)
{
    qDebug() << member << endl;

    /* Long Process!!! Data Transmission */
    /*QByteArray image_data;

    if( !image_data.isEmpty() )
    {
        is_fingerprint_captured_ = true;
        QImage image = QImage::fromData(image_data,"PNG");
        captured_image_ = image;
        ui->FingerprintCapturedLabel->setPixmap(QPixmap::fromImage(image));
    }
    else
    {
        is_fingerprint_captured_ = false;
    }*/
}

void FingerprintCaptureForm::on_FingerprintRetrievalFailure()
{

}

void FingerprintCaptureForm::on_FingerprintPostSuccessful()
{
    /* Display notification */
    QMessageBox message_box;
    message_box.setWindowOpacity(50);
    message_box.setWindowTitle("Fingerprint Captured");
    message_box.setStyleSheet("QLabel{ font-weight: plain; font-size: 14px; } \
                                 QPushButton{ width:125px; height:10; font-size: 14px; }");
    message_box.setStandardButtons(QMessageBox::Ok);

    message_box.setIcon(QMessageBox::Information);
    message_box.setText("Fingerprints Submission Complete");

    message_box.exec();

    /* disable to save portrait */
    ui->FingerprintCapturedLabel->clear();
    ui->FingerprintSavePushButton->setHidden(true);
    ui->FingerprintSaveFrame->setDisabled(true);
}

void FingerprintCaptureForm::on_FingerprintPostFailure()
{
    /* Display notification */
    QMessageBox message_box;
    message_box.setWindowOpacity(50);
    message_box.setWindowTitle("Fingerprint Captured");
    message_box.setStyleSheet("QLabel{ font-weight: plain; font-size: 14px; } \
                                 QPushButton{ width:125px; height:10; font-size: 14px; }");
    message_box.setStandardButtons(QMessageBox::Ok);

    message_box.setIcon(QMessageBox::Warning);
    message_box.setText("Fingerprints Submission Failed");

    message_box.exec();
}

void FingerprintCaptureForm::setPerson(QJsonObject &member, AdminMode mode)
{
    this->member_ = member;
    this->mode_ = mode;

    QJsonObject jsonSuccess = member[ "data" ].toObject();
    QString id = jsonSuccess.value("id").toString();

    api::instance()->getCapturedFingerprint(id, mode);
}
