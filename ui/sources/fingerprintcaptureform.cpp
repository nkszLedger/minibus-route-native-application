#include "ui\headers\fingerprintcaptureform.h"
#include "ui_fingerprintcaptureform.h"

FingerprintCaptureForm::FingerprintCaptureForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FingerprintCaptureForm)
{
    ui->setupUi(this);

    /* initialize fields */
    stored_image_counter_ = 0;
    c_buffer_size_        = 500;
    current_image_width_  = c_buffer_size_;
    current_image_height_ = c_buffer_size_;
    current_image_        = new (nothrow) unsigned char[current_image_width_*current_image_height_];

    /* set stop stream control status */
    stop_streaming_       = false;

    /* setup scanner */
    scanner_ = new FutronicScanner();

    /* connect statements */
    //connect(scanner_, SIGNAL(link(int)), this, SLOT(on_update_screen_during_streaming(int)));
    //connect(scanner_, SIGNAL(image_link(QByteArray,int)), this, SLOT(on_image_receive(QByteArray,int)));


    /*connect(UtilityFunction::instance(), \
          SIGNAL(continueToSaveFingerprints(QString)),\
          this, \
          SLOT(savePrints(QString)));*/
}

FingerprintCaptureForm::~FingerprintCaptureForm()
{
    delete ui;
}

// -------------
// Stream WIndow
// -------------
void FingerprintCaptureForm::on_update_screen_during_streaming(int flag)
{
  // clear display
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

      if(!stop_streaming_)
      {
        current_image_width_  = c_buffer_size_;
        current_image_height_ = c_buffer_size_;
      }
  }
  // update interface
  ui->FingerprintCaptureLabel->repaint();
  QCoreApplication::processEvents();
}
