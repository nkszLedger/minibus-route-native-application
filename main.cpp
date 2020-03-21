#include "basewindow.h"
#include "portraitcaptureform.h"
#include "verifymemberform.h"
#include "landingform.h"
#include "loginform.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*BaseWindow w;
    w.show();*/

    /*PortraitCaptureForm w;
    w.show();*/

    /*LoginForm w;
    w.show();*/

    VerifyMemberForm w;
    w.show();

    /*LandingForm w;
    w.show();*/

    return a.exec();
}
