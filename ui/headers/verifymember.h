#ifndef VERIFYMEMBER_H
#define VERIFYMEMBER_H

#include <QDialog>

namespace Ui {
class VerifyMember;
}

class VerifyMember : public QDialog
{
    Q_OBJECT

public:
    explicit VerifyMember(QWidget *parent = nullptr);
    ~VerifyMember();

private:
    Ui::VerifyMember *ui;
};

#endif // VERIFYMEMBER_H
