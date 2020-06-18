#ifndef PORTRAITCAPTUREFORM_H
#define PORTRAITCAPTUREFORM_H

#include <QWidget>
#include <QVector>
#include <QSqlRecord>
#include <QSqlField>
#include <QCamera>
#include <QJsonObject>
#include <QCameraImageCapture>

#include <api.h>
#include <person.h>

namespace Ui {
class PortraitCaptureForm;
}

class PortraitCaptureForm : public QWidget
{
    Q_OBJECT

public:
    explicit PortraitCaptureForm(QWidget *parent = nullptr);

    ~PortraitCaptureForm();

    void setPerson(QJsonObject &member, AdminMode mode);

    void setMember(const QString memberDbID);

signals:
    void home_button_clicked_signal(AdminMode mode);

public slots:
    /*!
     * \brief processCapturedImage
     * \param requestId
     * \param img
     */
    void processCapturedImage(int requestId, const QImage &img);

    /*!
     * \brief deleteTempDir
     * \param id
     * \param path
     */
    void deleteTempDir(int id, QString path);

    /*!
     * \brief savePrints
     * \param directory
     */
    void savePrints(QString directory);

private slots:
    void on_HomePushButton_clicked();

    void on_CapturePushButton_clicked();

    void on_PortraitSavePushButton_clicked();

    void on_PortraitRetrievalSuccessful(QJsonObject &member);
    void on_PortraitRetrievalFailure();
    void on_PortraitPostlFailure();
    void on_PortraitPostSuccessful();
private:
    Ui::PortraitCaptureForm *ui;

    /* Person Details */
    QString member_db_id_;
    QVector<QSqlRecord> member_;
    AdminMode mode_;

    /* Camera */
    QImage captured_image_;
    QCamera *camera_;
    QCameraImageCapture *image_capture_;
    bool is_capturing_image_;
    bool is_portrait_captured_;

    /* helper functions */
    void startCamera();
    void stopCamera();
    int scanDevices();
    void setCamera(const QCameraInfo &cameraInfo);
    void updateCaptureMode();
    void reset();
};

#endif // PORTRAITCAPTUREFORM_H
