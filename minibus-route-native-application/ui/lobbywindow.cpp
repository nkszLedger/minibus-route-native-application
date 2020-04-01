#include "lobbywindow.h"
#include "ui_lobbywindow.h"

LobbyWindow::LobbyWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LobbyWindow)
{
    ui->setupUi(this);

    /* init forms */
    login_form_ = new LoginForm();
    verify_member_form_ = new VerifyMemberForm();
    member_home_form_ = new MemberHomeForm();

    /* set steps */
    ui->stackedWidget->insertWidget(0, login_form_ );
    ui->stackedWidget->insertWidget(1, verify_member_form_ );
    ui->stackedWidget->insertWidget(2, member_home_form_ );

    /* set first step */
    init_step();

    /* connect signals & slots */
    connect(login_form_, SIGNAL(login_success_signal()), this, SLOT(go_to_verification_step()));
    connect(verify_member_form_, SIGNAL(verification_success()), this, SLOT(go_to_member_home_step()));

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

void LobbyWindow::go_to_member_home_step()
{
    ui->stackedWidget->setCurrentIndex(HOME);
}
