#ifndef LANDINGFORM_H
#define LANDINGFORM_H

#include <QDialog>

namespace Ui {
class LandingForm;
}

class LandingForm : public QDialog
{
    Q_OBJECT

public:
    explicit LandingForm(QWidget *parent = nullptr);
    ~LandingForm();

private:
    Ui::LandingForm *ui;
};

#endif // LANDINGFORM_H
