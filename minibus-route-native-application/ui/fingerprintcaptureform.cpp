#include "fingerprintcaptureform.h"
#include "ui_fingerprintcaptureform.h"

#include <QFileDialog>
#include <QMessageBox>
#include <UtilityFunction.h>

FingerprintCaptureForm::FingerprintCaptureForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FingerprintCaptureForm)
{
    ui->setupUi(this);

    /* External Process */
    process_ = new QProcess(parent);

    /* Program name  */
    program_ = "minibus-route-ftr-proc";

    connect(api::instance(), SIGNAL(details_found(QJsonObject &)),
                                this, SLOT(details_found(QJsonObject &)));
    connect(api::instance(), SIGNAL(details_not_found()),
                                this, SLOT(details_not_found()));

    connect(api::instance(), SIGNAL(fingerprints_post_success()),
                                this, SLOT(on_FingerprintPostSuccessful()));
    connect(api::instance(), SIGNAL(fingerprints_post_failure()),
                                this, SLOT(on_FingerprintPostlFailure()));

}

FingerprintCaptureForm::~FingerprintCaptureForm()
{
    delete ui;
}


void FingerprintCaptureForm::on_HomePushButton_clicked()
{
    if( this->mode_ == ADMINISTER_MEMBER )
    {
        emit home_button_clicked_signal(ADMINISTER_MEMBER);
    }
    else if( this->mode_ == ADMINISTER_MILITARY_VETERAN )
    {
        emit home_button_clicked_signal(ADMINISTER_MILITARY_VETERAN);
    }
    else{
        emit home_button_clicked_signal(ADMINISTER_EMPLOYEE);
    }
}

void FingerprintCaptureForm::on_FingerprintSavePushButton_clicked()
{
    /* Long Process!!! Data Transmission */
    if( process_ ) { process_->kill(); }

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Images (*.ansi *.iso)"));
    dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;

    if (dialog.exec())
        fileNames = dialog.selectedFiles();

    qDebug() << "File Path: " << fileNames;

    if( this->mode_ == ADMINISTER_MEMBER )
    { this->upload( fileNames[0], fileNames[1] ); }
    else { this->upload( fileNames.first() ); }
}

void FingerprintCaptureForm::upload(QString fileName1, QString fileName2)
{
    QByteArray f_data1;
    QByteArray f_data2;
    QFile f_file1(fileName1);
    QFile f_file2(fileName2);

    /* check if first file can be read */
    if( !f_file1.open(QIODevice::ReadOnly) )
    {
        qDebug() << "Could not read first file";
    }
    else
    {
        /* read all data into byte array */
        QBuffer buffer(&f_data1);
        buffer.open(QBuffer::ReadWrite);
        buffer.write(f_file1.readAll());
        /* close file */
        f_file1.close();
    }

    /* check if second file can be read */
    if( !f_file2.open(QIODevice::ReadOnly) )
    {
        qDebug() << "Could not read second file";
    }
    else
    {
        /* read all data into byte array */
        QBuffer buffer(&f_data2);
        buffer.open(QBuffer::ReadWrite);
        buffer.write(f_file2.readAll());
        /* close file */
        f_file2.close();
    }

    if( this->mode_ == ADMINISTER_MEMBER )
    {
        if( !f_data1.isEmpty() && !f_data2.isEmpty() )
        {
            api::instance()->postCapturedFingerprint(this->member_db_id_,
            f_data1, f_data2, "member_fingerprint", this->mode_, is_fingerprint_captured_ );
        }
        else
        {
            qDebug() << "Member fingerprint Files missing data could not upload to DB";
        }
    }
}

void FingerprintCaptureForm::upload(QString fileName)
{
    QByteArray data;
    qDebug() << "FileName: " << fileName;
    QFile file(fileName);
    qDebug() << "Mode: " << mode_;
    qDebug() << "Fingerprint Image Size 1: " << data.length();

    /* check if first file can be read */
    if( !file.open(QIODevice::ReadOnly) )
    {
        qDebug() << "Could not read first file";
    }
    else
    {
        /* read all data into byte array */
        data = file.readAll()   ;
        /* close file */
        file.close();
    }

    qDebug() << "Fingerprint Image Size 2: " << data.length();

    if( this->mode_ == ADMINISTER_EMPLOYEE )
    {
        api::instance()->postCapturedFingerprint(this->employee_db_id_,
        data, data,"employee_fingerprint",this->mode_,is_fingerprint_captured_ );
    }
    else
    {
        /* ADMINISTER_MILITARY_VETERAN  */
        api::instance()->postCapturedFingerprint(this->military_veteran_db_id_,
        data, data,"military_veteran_fingerprints", this->mode_,is_fingerprint_captured_ );
    }

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

void FingerprintCaptureForm::details_found(QJsonObject &)
{

}

void FingerprintCaptureForm::details_not_found()
{

}

void FingerprintCaptureForm::setPerson(QJsonObject &member, AdminMode mode)
{
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
        /*QImage image = QImage::fromData(image_data,"ANSI");
        qleft_thumb_image_ = image;*/
        ui->FingerprintCapturedLabel->setPixmap(
        QPixmap::fromImage(QImage(":/resources/icons/fingerprint_captured.jpeg")));
        ui->activityLabel->setText("Previous Fingerprint Exists!");
    }
    else
    {
        qDebug() << "FingerprintCaptureForm::setEmployee()"
                 << " - Images empty";
        is_fingerprint_captured_ = false;
        ui->FingerprintCapturedLabel->setStyleSheet("background-color: rgb(0, 0, 0);");
        ui->activityLabel->setText("Previous Fingerprint does not exist!");
    }
}

void FingerprintCaptureForm::setMilitaryVeteran(QString militaryVeteranDbID)
{
    this->military_veteran_db_id_ = militaryVeteranDbID;
    this->mode_ = ADMINISTER_MILITARY_VETERAN;

    QByteArray image_data;

    api::instance()->getCapturedFingerprintFromDB(this->military_veteran_db_id_,
                                                  image_data, image_data,
                                                  this->mode_,
                                                  "military_veteran_fingerprints");

    if( !image_data.isEmpty() )
    {
        is_fingerprint_captured_ = true;
        /*QImage image = QImage::fromData(image_data,"PNG");
        qleft_thumb_image_ = image;
        ui->FingerprintCapturedLabel->setPixmap(QPixmap::fromImage(image));*/
        ui->FingerprintCapturedLabel->setPixmap(
        QPixmap::fromImage(QImage(":/resources/icons/fingerprint_captured.jpeg")));
        ui->activityLabel->setText("Previous Fingerprint Exists!");
        /*createTemporaryDirectory(image_data);*/

    }
    else
    {
        qDebug() << "FingerprintCaptureForm::setMilitaryVeteran()"
                 << " - Images empty";
        is_fingerprint_captured_ = false;
        ui->FingerprintCapturedLabel->setStyleSheet("background-color: rgb(0, 0, 0);");
        ui->activityLabel->setText("Previous Fingerprint does not exist!");
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
        /*QImage image = QImage::fromData(left_thumb_image,"PNG");
        qleft_thumb_image_ = image;*/
        ui->FingerprintCapturedLabel->setPixmap(
        QPixmap::fromImage(QImage(":/resources/icons/fingerprint_captured.jpeg")));
        ui->activityLabel->setText("Previous Fingerprints Exists!");
    }

    if( !right_thumb_image.isEmpty() )
    {
        is_fingerprint_captured_ = true;
        /*QImage image = QImage::fromData(right_thumb_image,"PNG");
        qright_thumb_image_ = image;*/
        ui->FingerprintCapturedLabel->setPixmap(
        QPixmap::fromImage(QImage(":/resources/icons/fingerprint_captured.jpeg")));
        ui->activityLabel->setText("Previous Fingerprints Exists!");
    }

    if( left_thumb_image.isEmpty() &&
            right_thumb_image.isEmpty() )
    {
        qDebug() << "FingerprintCaptureForm::setMember()"
                 << " - Images empty";
        is_fingerprint_captured_ = false;
        ui->FingerprintCapturedLabel->setStyleSheet("background-color: rgb(0, 0, 0);");
        ui->activityLabel->setText("Previous Fingerprint does not exist!");
    }


}

void FingerprintCaptureForm::on_CapturePushButton_clicked()
{
    /* Start program  */
    process_->start(program_);
    process_->write("Fingerprint Capture Started");
}

void FingerprintCaptureForm::createTemporaryDirectory(QByteArray &data)
{
    QStringList path = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);
    QString new_path  =  path.at(0) + "/MRPTRMSClientOutDir/";
    qDebug() << "The Path: " << new_path;
    directory_.mkdir(directory_.path());

    QFile file(new_path + "image00000011.ansi");
    file.open(QIODevice::WriteOnly);
    file.write(data);
    file.close();
}
