#include "fingerprintcaptureform.h"
#include "ui_fingerprintcaptureform.h"

#include <QMessageBox>
#include <UtilityFunction.h>

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
    if( this->mode_ == ADMINISTER_MEMBER )
    { emit home_button_clicked_signal(ADMINISTER_MEMBER);}
    else{ emit home_button_clicked_signal(ADMINISTER_EMPLOYEE);}
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
        scanner_->stream(current_image_, current_image_width_,
                         current_image_height_, stop_streaming_ );
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
            ui->ImageOnelabel->setPixmap(QPixmap::fromImage(captured_image_));
        }
        else if( ui->RightRadioButton->isChecked() )
        {
            is_right_thumb_captured_ = true;
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

        QImage image(current_finger.image,\
                     current_finger.image_width,\
                     current_finger.image_height,\
                     QImage::Format_Indexed8);

        /* save image captured */
        if( ui->LeftRadioButton->isChecked() )
        {
            left_thumb_image_ = current_finger;
            qleft_thumb_image_ = image;
        }
        else if( ui->RightRadioButton->isChecked() )
        {
            right_thumb_image_ = current_finger;
            qright_thumb_image_ = image;
        }

        captured_image_ = image;
    }

    /* test update stream from here */
    on_update_screen_during_streaming(quality);
}

void FingerprintCaptureForm::on_FingerprintSavePushButton_clicked()
{
    /* Long Process!!! Data Transmission */
    unsigned int DPI         = 500;
    unsigned int BPP         = 8;

    if( this->mode_ == ADMINISTER_MEMBER )
    {
        if( is_left_thumb_captured_ &&
              is_right_thumb_captured_ )
        {
            QByteArray left_thumb_image_data;
            QString left_finger_file_name = QString("image1.png");

            QByteArray right_thumb_image_data;
            QString right_finger_file_name = QString("image2.png");

            QBuffer lbuffer(&left_thumb_image_data);
            lbuffer.open(QIODevice::WriteOnly);
            qleft_thumb_image_.save(&lbuffer, "PNG");

            QBuffer rbuffer(&right_thumb_image_data);
            rbuffer.open(QIODevice::WriteOnly);
            qright_thumb_image_.save(&rbuffer, "PNG");

            unsigned int save_status_one = 0;
            /*UtilityFunction::instance()
                ->save_to_wsq( left_thumb_image_.image,\
                               left_thumb_image_.image_width,\
                               left_thumb_image_.image_height,\
                               DPI, \
                               BPP,\
                               left_finger_file_name );*/

            unsigned int save_status_two = 0;
            /*UtilityFunction::instance()
                ->save_to_wsq( right_thumb_image_.image,\
                               right_thumb_image_.image_width,\
                               right_thumb_image_.image_height,\
                               DPI, \
                               BPP,\
                               right_finger_file_name );*/

            qDebug() << "on_FingerprintSavePushButton_clicked() - "
                     << "Left FPrint Status: "
                     << save_status_one;
            qDebug() << "on_FingerprintSavePushButton_clicked() - "
                     << "Left FPrint Status: "
                     << save_status_two;

            qDebug() << "left_thumb_image_data isEmpty: "
                     << left_thumb_image_data.isEmpty();

            api::instance()->postCapturedFingerprint(this->member_db_id_,
                                                     left_thumb_image_data,
                                                     right_thumb_image_data,
                                                     "member_fingerprint",
                                                     this->mode_,
                                                     is_fingerprint_captured_ );

            /* clean up */
            UtilityFunction::instance()->deletePath(left_finger_file_name);
            UtilityFunction::instance()->deletePath(right_finger_file_name);

        }
        else
        {
            /* Display notification */
            QMessageBox message_box;
            message_box.setWindowOpacity(50);
            message_box.setWindowTitle("Fingerprint Capture");
            message_box.setStyleSheet("QLabel{ font-weight: plain; font-size: 14px; } \
                                         QPushButton{ width:125px; height:10; font-size: 14px; }");
            message_box.setStandardButtons(QMessageBox::Ok);

            message_box.setIcon(QMessageBox::Warning);
            message_box.setText("Please capture at most two fingers");

            message_box.exec();
        }
    }
    else
    {

        unsigned int save_status = 0;/*UtilityFunction::instance()
                                    ->save_to_wsq( current_finger_.image,\
                                                   current_finger_.image_width,\
                                                   current_finger_.image_height,\
                                                   DPI, \
                                                   BPP,\
                                                   fileName );*/


        qDebug() << "on_FingerprintSavePushButton_clicked() - FPrint Status: " << save_status;

        QByteArray image_data;

        QString fileName = QString("image1.png");

        QBuffer buffer(&image_data);
        buffer.open(QIODevice::WriteOnly);
        captured_image_.save(&buffer, "PNG");

        api::instance()->postCapturedFingerprint(this->employee_db_id_,
                                                 image_data, image_data,
                                                 "employee_fingerprint",
                                                 this->mode_,
                                                 is_fingerprint_captured_ );

        /* clean up */
        UtilityFunction::instance()->deletePath(fileName);

    }

    /* clear captured content */
    ui->FingerprintCapturedLabel->clear();
    is_left_thumb_captured_ = false;
    is_right_thumb_captured_ = false;
}

void FingerprintCaptureForm::on_FingerprintRetrievalSuccessful(QJsonObject &member)
{
    QJsonObject jsonSuccess = member[ "data" ].toObject();

    if( this->mode_ == ADMINISTER_MEMBER )
    {
        /*QJsonArray left_thumb = jsonSuccess.value("fingerprint_left_thumb").toArray();
        QJsonArray right_thumb = jsonSuccess.value("fingerprint_right_thumb").toArray();
        qDebug() << "left_thumb" << endl;
        qDebug() << left_thumb << endl;*/
    }
    else
    {

    }
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
    message_box.setStyleSheet("QLabel{ font-weight: plain; \
                                font-size: 14px; } \
                                 QPushButton{ width:125px; \
                                    height:10; font-size: 14px; }");
    message_box.setStandardButtons(QMessageBox::Ok);

    message_box.setIcon(QMessageBox::Information);
    message_box.setText("Fingerprints Submission Complete");

    message_box.exec();

    /* disable to save portrait */
    ui->ImageOnelabel->clear();
    ui->ImageTwolabel->clear();
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
    message_box.setStyleSheet("QLabel{ font-weight: plain; \
                                    font-size: 14px; } \
                                 QPushButton{ width:125px; \
                                    height:10; font-size: 14px; }");
    message_box.setStandardButtons(QMessageBox::Ok);

    message_box.setIcon(QMessageBox::Warning);
    message_box.setText("Fingerprints Submission Failed");

    message_box.exec();
}

void FingerprintCaptureForm::setPerson(QJsonObject &member, AdminMode mode)
{
    /*this->member_ = member; */
    this->mode_ = mode;

    QJsonObject jsonSuccess = member[ "data" ].toObject();
    QString id = jsonSuccess.value("id").toString();

    api::instance()->getCapturedFingerprint(id, mode);
}

void FingerprintCaptureForm::setEmployee(QString employeeDbID)
{
    this->employee_db_id_ = employeeDbID;
    this->mode_ = ADMINISTER_EMPLOYEE;

    QByteArray image_data;

    api::instance()->getCapturedFingerprintFromDB(this->employee_db_id_,
                                                  image_data, image_data,
                                                  this->mode_,
                                                  "employee_fingerprint");

    if( !image_data.isEmpty() )
    {
        is_fingerprint_captured_ = true;
        QImage image = QImage::fromData(image_data,"PNG");
        qleft_thumb_image_ = image;
        ui->FingerprintCapturedLabel->setPixmap(QPixmap::fromImage(image));
    }
    else
    {
        qDebug() << "FingerprintCaptureForm::setEmployee()"
                 << " - Images empty";
        is_fingerprint_captured_ = false;
    }
}

void FingerprintCaptureForm::setMember(const QString memberDbID)
{
    this->member_db_id_ = memberDbID;
    this->mode_ = ADMINISTER_MEMBER;

    /* Long Process!!! Data Transmission */

    QByteArray left_thumb_image;
    QByteArray right_thumb_image;

    api::instance()->getCapturedFingerprintFromDB(this->member_db_id_,
                                                  left_thumb_image,
                                                  right_thumb_image,
                                                  this->mode_,
                                                  "member_fingerprint");

    if( !left_thumb_image.isEmpty() )
    {
        is_fingerprint_captured_ = true;
        QImage image = QImage::fromData(left_thumb_image,"PNG");
        qleft_thumb_image_ = image;
        ui->ImageOnelabel->setPixmap(QPixmap::fromImage(image));
    }

    if( !right_thumb_image.isEmpty() )
    {
        is_fingerprint_captured_ = true;
        QImage image = QImage::fromData(right_thumb_image,"PNG");
        qright_thumb_image_ = image;
        ui->ImageTwolabel->setPixmap(QPixmap::fromImage(image));
    }

    if( left_thumb_image.isEmpty() &&
            right_thumb_image.isEmpty() )
    {
        qDebug() << "FingerprintCaptureForm::setMember()"
                 << " - Images empty";
        is_fingerprint_captured_ = false;
    }


}
