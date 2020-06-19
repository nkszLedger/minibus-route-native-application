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
    connect(&login_form_, SIGNAL(login_success_signal()),
                this, SLOT(go_to_member_verification_step()));
    connect(&login_form_, SIGNAL(close_application_signal()),
                this, SLOT(close_application()));

    /* 2 - HOME FORM: connect signals & slots */
    connect(&home_form_, SIGNAL(user_verification_signal()),
                this, SLOT(go_to_user_verification_step()));
    connect(&home_form_, SIGNAL(member_verification_signal()),
                this, SLOT(go_to_member_verification_step()));

    /* 3 - VERIFY MEMBER FORM: connect signals & slots */
    connect(&verify_member_form_,
                SIGNAL(verification_success_signal(QJsonObject&, AdminMode)),
                this, SLOT(go_to_member_home_step(QJsonObject&, AdminMode)));

    /* 3 - VERIFY USER FORM: connect signals & slots */
    connect(&verify_user_form_,
                SIGNAL(verification_success_signal(QJsonObject&)),
                this, SLOT(go_to_member_home_step(QJsonObject&)));

    /* 4 - MEMBER HOME FORM: connect signals & slots */
    connect(&member_home_form_,
                SIGNAL(back_button_clicked_signal()),
                this, SLOT(go_to_member_verification_step()));
    connect(&member_home_form_,
                SIGNAL(fingerprint_capture_clicked_signal(QJsonObject&, AdminMode)),
                this, SLOT(go_to_capture_fingerprint_step(QJsonObject&, AdminMode)));
    connect(&member_home_form_,
                SIGNAL(portrait_capture_clicked_signal(QJsonObject&, AdminMode)),
                this, SLOT(go_to_capture_portrait_step(QJsonObject&, AdminMode)));

    /* 5 - FINGERPRINT CAPTURE FORM: connect signals & slots */
    connect(&fingerprint_capture_,
                SIGNAL(home_button_clicked_signal(AdminMode)),
                this, SLOT(go_to_member_home_choice(AdminMode)));

    /* 6 - PORTRAIT CAPTURE FORM: connect signals & slots */
    connect(&portrait_capture_,
                SIGNAL(home_button_clicked_signal(AdminMode)),
                this, SLOT(go_to_member_home_choice(AdminMode)));

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

void LobbyWindow::refresh(QJsonObject &person, AdminMode mode)
{
    QJsonObject jsonSuccess = person[ "data" ].toObject();
    QString id = jsonSuccess.value("id").toString();

    /*portrait_capture_.setPerson(person, mode);
    fingerprint_capture_.setPerson(person, mode);*/

    member_home_form_.setPerson(person, mode);

    if( mode == ADMINISTER_MEMBER )
    {
        portrait_capture_.setMember(id);
        fingerprint_capture_.setMember(id);
    }
    else
    {
        portrait_capture_.setEmployee(id);
        fingerprint_capture_.setEmployee(id);
    }

}

void LobbyWindow::go_to_member_home_step(QJsonObject &person, AdminMode mode)
{
    this->current_person_ = person;
    refresh(person, mode);
    ui->stackedWidget->setCurrentIndex(MEMBERHOME);
}

void LobbyWindow::go_to_member_home_choice(AdminMode mode)
{
    refresh(this->current_person_, mode);
    ui->stackedWidget->setCurrentIndex(MEMBERHOME);
}

void LobbyWindow::go_to_capture_fingerprint_step(QJsonObject &person, AdminMode mode)
{
    refresh(person, mode);
    ui->stackedWidget->setCurrentIndex(FINGERPRINTCAPTURE);
}

void LobbyWindow::go_to_capture_portrait_step(QJsonObject &person, AdminMode mode)
{
    refresh(person, mode);
    ui->stackedWidget->setCurrentIndex(PORTRAITCAPTURE);
}
