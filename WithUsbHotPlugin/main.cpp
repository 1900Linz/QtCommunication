#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include "DevPluginUSB.h"

#include <Windows.h>
#include <Dbt.h>
#include <devguid.h>
#include <initguid.h>
#include <usbiodef.h>
#include <hidclass.h>
#include <ntddkbd.h>
#include <ntddmou.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DevPluginUSB filter;

    QVector<QUuid> uuids;
    uuids << GUID_DEVINTERFACE_USB_DEVICE;
    filter.init(uuids);

    QObject::connect(&filter, &DevPluginUSB::message,
                     &app, [](QString msg){
        qDebug()<<msg;
    });


    return app.exec();
}
