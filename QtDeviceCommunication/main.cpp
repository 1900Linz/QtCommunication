#include "QtDeviceCommunication.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtDeviceCommunication w;
    w.show();
    return a.exec();
}
