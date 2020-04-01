#ifndef PORTRAITCAPTUREFORM_H
#define PORTRAITCAPTUREFORM_H

#include <QWidget>

namespace Ui {
class PortraitCaptureForm;
}

class PortraitCaptureForm : public QWidget
{
    Q_OBJECT

public:
    explicit PortraitCaptureForm(QWidget *parent = nullptr);
    ~PortraitCaptureForm();

private:
    Ui::PortraitCaptureForm *ui;
};

#endif // PORTRAITCAPTUREFORM_H
