#include "lobbywindow.h"
#include "ui_lobbywindow.h"

LobbyWindow::LobbyWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LobbyWindow)
{
    ui->setupUi(this);

    /* set steps */
    ui->stackedWidget->insertWidget(LOGIN, &login_form_ );
    ui->stackedWidget->insertWidget(MEMBERVERIFICATION, &verify_member_form_ );
    ui->stackedWidget->insertWidget(USERVERIFICATION, &verify_user_form_ );
    ui->stackedWidget->insertWidget(HOME, &home_form_ );
    ui->stackedWidget->insertWidget(MEMBERHOME, &member_home_form_ );
    ui->stackedWidget->insertWidget(FINGERPRINTCAPTURE, &fingerprint_capture_);
    ui->stackedWidget->insertWidget(PORTRAITCAPTURE, &portrait_capture_ );

    /* set first step */
    init_step();

    /* 1 - LOGIN FORM: connect signals & slots */
    connect(&login_form_, SIGNAL(login_success_signal()), this, SLOT(go_to_home_step()));
    connect(&login_form_, SIGNAL(close_application_signal()), this, SLOT(close_application()));

    /* 2 - HOME FORM: connect signals & slots */
    connect(&home_form_, SIGNAL(user_verification_signal()), this, SLOT(go_to_user_verification_step()));
    connect(&home_form_, SIGNAL(member_verification_signal()), this, SLOT(go_to_member_verification_step()));

    /* 3 - VERIFY MEMBER FORM: connect signals & slots */
    connect(&verify_member_form_, SIGNAL(verification_success_signal(Person*)),
                                this, SLOT(go_to_member_home_step(Person*)));

    /* 3 - VERIFY USER FORM: connect signals & slots */
    connect(&verify_user_form_, SIGNAL(verification_success_signal(Person*)),
                                this, SLOT(go_to_member_home_step(Person*)));

    /* 4 - MEMBER HOME FORM: connect signals & slots */
    connect(&member_home_form_, SIGNAL(back_button_clicked_signal()),
                                this, SLOT(go_to_member_verification_step()));
    connect(&member_home_form_, SIGNAL(fingerprint_capture_clicked_signal(Person*)),
                                this, SLOT(go_to_capture_fingerprint_step(Person*)));
    connect(&member_home_form_, SIGNAL(portrait_capture_clicked_signal(Person*)),
                                this, SLOT(go_to_capture_portrait_step(Person*)));

    /* 5 - FINGERPRINT CAPTURE FORM: connect signals & slots */
    connect(&fingerprint_capture_, SIGNAL(home_button_clicked_signal(Person*)),
                                this, SLOT(go_to_member_home_step(Person*)));

    /* 6 - PORTRAIT CAPTURE FORM: connect signals & slots */
    connect(&portrait_capture_, SIGNAL(home_button_clicked_signal(Person*)),
                                this, SLOT(go_to_member_home_step(Person*)));



}

LobbyWindow::~LobbyWindow()
{
    delete ui;
}

void LobbyWindow::close_application()
{
    this->close();
}

void LobbyWindow::init_step()
{
    ui->stackedWidget->setCurrentIndex(LOGIN);
}

void LobbyWindow::go_to_home_step()
{
    ui->stackedWidget->setCurrentIndex(HOME);
}

void LobbyWindow::go_to_user_verification_step()
{
    ui->stackedWidget->setCurrentIndex(USERVERIFICATION);
}


void LobbyWindow::go_to_member_verification_step()
{
    ui->stackedWidget->setCurrentIndex(MEMBERVERIFICATION);
}

void LobbyWindow::refresh(Person *person)
{
    member_home_form_.setPerson(person);
    portrait_capture_.setPerson(person);
    fingerprint_capture_.setPerson(person);
}

void LobbyWindow::go_to_member_home_step(Person *person)
{
    refresh(person);
    ui->stackedWidget->setCurrentIndex(MEMBERHOME);
}

void LobbyWindow::go_to_capture_fingerprint_step(Person *person)
{
    refresh(person);
    ui->stackedWidget->setCurrentIndex(FINGERPRINTCAPTURE);
}

void LobbyWindow::go_to_capture_portrait_step(Person *person)
{
    refresh(person);
    ui->stackedWidget->setCurrentIndex(PORTRAITCAPTURE);
}
