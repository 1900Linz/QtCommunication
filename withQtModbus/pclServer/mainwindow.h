#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpServer>
#include<QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpServer *tcpserver;
    QTcpSocket *tcpsocket;

private slots:
    void connectSlot();
    void readData();
    void disConnectSlot();
    void on_send_clicked();
     void StateChanged(QAbstractSocket::SocketState socketState);

};
#endif // MAINWINDOW_H
