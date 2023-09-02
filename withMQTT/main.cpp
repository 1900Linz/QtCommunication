#include <QCoreApplication>
#include "ClientSubscription.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ClientSubscription mqttModule;
    return a.exec();
}
