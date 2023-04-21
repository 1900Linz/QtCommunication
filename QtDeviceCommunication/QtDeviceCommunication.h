#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtDeviceCommunication.h"

class QtDeviceCommunication : public QMainWindow
{
    Q_OBJECT

public:
    QtDeviceCommunication(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtDeviceCommunicationClass ui;
};
