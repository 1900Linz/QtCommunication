#include "cameradisplay.h"
#include "ui_cameradisplay.h"

cameraDisplay::cameraDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cameraDisplay)
{
    ui->setupUi(this);
}

cameraDisplay::~cameraDisplay()
{
    delete ui;
}
