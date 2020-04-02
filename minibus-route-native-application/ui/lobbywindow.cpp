#include "lobbywindow.h"
#include "ui_lobbywindow.h"

LobbyWindow::LobbyWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LobbyWindow)
{
    ui->setupUi(this);

    /* set steps */
    ui->stackedWidget->insertWidget(LOGIN, &login_form_ );
    ui->stackedWidget->insertWidget(VERIFICATION, &verify_member_form_ );
    ui->stackedWidget->insertWidget(HOME, &member_home_form_ );
    ui->stackedWidget->insertWidget(FINGERPRINTCAPTURE, &fingerprint_capture_);
    ui->stackedWidget->insertWidget(PORTRAITCAPTURE, &portrait_capture_ );

    /* set first step */
    init_step();

    /* connect signals & slots */
    connect(&login_form_, SIGNAL(login_success_signal()), this, SLOT(go_to_verification_step()));
    connect(&verify_member_form_, SIGNAL(verification_success_signal(int)), this, SLOT(go_to_member_home_step(int)));
    connect(&member_home_form_, SIGNAL(back_button_clicked_signal()), this, SLOT(go_to_verification_step()));
    connect(&member_home_form_, SIGNAL(fingerprint_capture_clicked_signal(QString)), this, SLOT(go_to_capture_fingerprint_step(QString)));
    connect(&member_home_form_, SIGNAL(portrait_capture_clicked_signal(QString)), this, SLOT(go_to_capture_portrait_step(QString)));
    connect(&fingerprint_capture_, SIGNAL(home_button_clicked_signal(int)), this, SLOT(go_to_member_home_step(int)));
    connect(&portrait_capture_, SIGNAL(home_button_clicked_signal(int)), this, SLOT(go_to_member_home_step(int)));

}

LobbyWindow::~LobbyWindow()
{
    delete ui;
}

void LobbyWindow::init_step()
{
    ui->stackedWidget->setCurrentIndex(LOGIN);
}

void LobbyWindow::go_to_verification_step()
{
    ui->stackedWidget->setCurrentIndex(VERIFICATION);
}

void LobbyWindow::go_to_member_home_step(int step)
{
    ui->stackedWidget->setCurrentIndex(HOME);

//    if( step == FINGERPRINTCAPTURE)
//        ui->stackedWidget->removeWidget(&fingerprint_capture_);
//    else if( step == PORTRAITCAPTURE)
//        ui->stackedWidget->removeWidget(&portrait_capture_);
//    else
//    {

//    }

}
void LobbyWindow::go_to_capture_fingerprint_step(QString member_id)
{
    ui->stackedWidget->setCurrentIndex(FINGERPRINTCAPTURE);
}
void LobbyWindow::go_to_capture_portrait_step(QString member_id)
{
    ui->stackedWidget->setCurrentIndex(PORTRAITCAPTURE);
}
