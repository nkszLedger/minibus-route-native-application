#ifndef FINGERPRINTCAPTUREFORM_H
#define FINGERPRINTCAPTUREFORM_H

#include <QDialog>
#include <QImage>
#include <QWidget>
#include <scanner.h>
#include <UtilityFunction.h>
#include <futronic_scanner.h>

namespace Ui {
class FingerprintCaptureForm;
}

class FingerprintCaptureForm : public QDialog
{
    Q_OBJECT

public:
    explicit FingerprintCaptureForm(QWidget *parent = nullptr);
    ~FingerprintCaptureForm();

public slots:

    /*!
    * \brief savePrints
    * \param filePath
    */
    void savePrints(QString filePath);
    /*!
    * \brief on_update_screen_during_streaming
    * \param Flag
    */
    void on_update_screen_during_streaming(int flag);

    /*!
    * \brief receives image from stream and verifies if image should be stored
    * \param Flag
    */
    void on_image_receive(QByteArray Image,int quality);

private:
    Ui::FingerprintCaptureForm *ui;
    Scanner* scanner_;

    bool stop_streaming_;
    unsigned char* current_image_;
    int c_buffer_size_ ;
    int current_image_width_;
    int current_image_height_;
    int stored_image_counter_;

};

#endif // FINGERPRINTCAPTUREFORM_H
