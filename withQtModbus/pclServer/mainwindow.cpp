#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("服务器端");
    resize(800,600);

    //初始化QTcpServer对象
    tcpserver=new QTcpServer(this);
    //设置监听IP和端口，这里的any指本机上面的所有网卡地址，listen方法一经调用就会开始循环监听客户端的连接了，
    tcpserver->listen(QHostAddress::Any,8899);

    //当有客户端连接过来时，就会触发newConnect信号,我们把信号绑定到槽函数上面去，便于实现连接后的逻辑
    connect(tcpserver,SIGNAL(newConnection()),this,SLOT(connectSlot()));

    //点击按钮与客户端断开连接
    connect(ui->dis_client,SIGNAL(clicked(bool)),this,SLOT(disConnectSlot()));


    //监测连接状态变化
    //connect(tcpsocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(StateChanged(QAbstractSocket::SocketState)));

    ui->send->setEnabled(false); //程序启动禁止点击发送消息按钮
    ui->dis_client->setEnabled(false); //程序启动禁用于断开连接按钮

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSlot()
{
    //获取socket对象
    tcpsocket=tcpserver->nextPendingConnection();
     //把对方的IP地址和端口显示出来
    auto ip = tcpsocket->peerAddress().toString();
    auto port = tcpsocket->peerPort();
    ui->look_message->append("与客户端建立连接成功！IP地址为："+ip+",端口号为："+QString::number(port));

    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(readData()));

    ui->send->setEnabled(true); //重新建立客户端连接后，启用点击发送消息按钮
    ui->dis_client->setEnabled(true);//重新建立客户端连接后，启用断开连接按钮

}

void MainWindow::readData()
{
    ui->look_message->append("客户端说："+tcpsocket->readAll());
}


void MainWindow::disConnectSlot()
{
   tcpsocket->disconnectFromHost();
   tcpsocket->close();
   tcpsocket=NULL;
   ui->look_message->append("与客户端断开连接成功！");
   ui->send->setEnabled(false); //断开客户端连接后，禁止点击发送消息按钮
   ui->dis_client->setEnabled(false); //点击一次断开客户端连接后，禁用于断开连接按钮
}


void MainWindow::on_send_clicked()
{
    //获取需要发送的文本
    QString str=ui->input_message->toPlainText().trimmed();



        if(str.length()>0 ){
        tcpsocket->write(str.toUtf8().data());
        ui->look_message->append("我给客户端说："+str);
        }else {

            ui->statusbar->showMessage("提示：不能发送空消息哦！");
        }

}

void MainWindow::StateChanged(QAbstractSocket::SocketState socketState)
{
//    QAbstractSocket::UnconnectedState  0       The socket is not connected.
//    QAbstractSocket::HostLookupState   1       The socket is performing a host name lookup.
//    QAbstractSocket::ConnectingState   2       The socket has started establishing a connection.
//    QAbstractSocket::ConnectedState    3       A connection is established.
//    QAbstractSocket::BoundState        4       The socket is bound to an address and port.
//    QAbstractSocket::ClosingState      6       The socket is about to close (data may still be waiting to be written).
//    QAbstractSocket::ListeningState    5       For internal use only.

    switch (socketState) {
    case QAbstractSocket::UnconnectedState:
        ui->statusbar->showMessage("The socket is not connected");
        break;

    case QAbstractSocket::HostLookupState:
        ui->statusbar->showMessage("The socket is performing a host name lookup");
        break;
    case QAbstractSocket::ConnectingState:
        ui->statusbar->showMessage("The socket has started establishing a connection.");
        break;
    case QAbstractSocket::ConnectedState :
        ui->statusbar->showMessage("A connection is established");
        break;
    case QAbstractSocket::BoundState :
        ui->statusbar->showMessage("The socket is bound to an address and port.");
        break;
    case QAbstractSocket::ClosingState:
        ui->statusbar->showMessage("The socket is about to close (data may still be waiting to be written).");
        break;
    case QAbstractSocket::ListeningState :
        ui->statusbar->showMessage("For internal use only");
        break;

    }
}

