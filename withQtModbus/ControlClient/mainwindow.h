#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void StartConnect();
    void connectSucc();
    void ReadData();
    void disConnect();
    void sendMessage();


    void on_send_message_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpsocket;
};
#endif // MAINWINDOW_H
