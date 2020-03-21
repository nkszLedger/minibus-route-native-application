#include "basewindow.h"
#include "portraitcaptureform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*BaseWindow w;
    w.show();*/

    PortraitCaptureForm w;
    w.show();

    return a.exec();
}
