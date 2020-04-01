#include "fingerprintcaptureform.h"
#include "ui_fingerprintcaptureform.h"

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

    /* connect statements */
    connect(scanner_, SIGNAL(link(int)), this, SLOT(on_update_screen_during_streaming(int)));
    connect(scanner_, SIGNAL(image_link(QByteArray,int)), this, SLOT(on_image_receive(QByteArray,int)));
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
    emit home_button_clicked_signal();
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
    /* save image */
    QString current_file_name = QString("%1.png")
                                .arg(QDateTime::currentDateTime().toString("yyMMdd_hh-mm-ss"));
    captured_image_.save(current_file_name,"png");

    /* clear captured content */
    ui->FingerprintCapturedLabel->clear();
}
