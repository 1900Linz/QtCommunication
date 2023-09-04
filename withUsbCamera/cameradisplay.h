#ifndef CAMERADISPLAY_H
#define CAMERADISPLAY_H

#include <QWidget>

namespace Ui {
class cameraDisplay;
}

class cameraDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit cameraDisplay(QWidget *parent = nullptr);
    ~cameraDisplay();

private:
    Ui::cameraDisplay *ui;
};

#endif // CAMERADISPLAY_H
