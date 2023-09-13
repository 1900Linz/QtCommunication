#include "mainwindow.h"
#include "ui_mainwindow.h"

#pragma
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("客户端"));
    resize(800,700);

    //设置默认的IP地址和端口
    ui->ip->setText("127.0.0.1");
    ui->port->setText("8899");

  tcpsocket=new QTcpSocket(this);

  //开始连接
  connect(ui->connect_bt,SIGNAL(clicked(bool)),this,SLOT(StartConnect()));

  //绑定客户端的连接信号与槽，便于在槽函数中书写逻辑
  connect(tcpsocket,SIGNAL(connected()),this,SLOT(connectSucc()));

  //当服务器端发送消息过来时，客户端接收并出来消息
  connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(ReadData()));

  //断开连接时处理业务
  connect(ui->disconnect_bt,SIGNAL(clicked(bool)),this,SLOT(disConnect()));


  ui->send_message->setEnabled(false);
  ui->disconnect_bt->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::StartConnect()
{
    //主动连接服务器
   tcpsocket->connectToHost(ui->ip->text().trimmed(),ui->port->text().toInt());
   ui->display_message->append("开始连接服务器");


}

void MainWindow::connectSucc()
{
    //if(tcpsocket->state()==QAbstractSocket::HostLookupState){}
    ui->display_message->append("服务器连接成功！");
    ui->display_message->append("现在可以开始互相发送消息啦！");
    ui->connect_bt->setEnabled(false);


    ui->send_message->setEnabled(true);
    ui->disconnect_bt->setEnabled(true);


}

void MainWindow::ReadData()
{
    //获取服务器端发送过来的内容
   auto data = tcpsocket->readAll();
   ui->display_message->append("服务器端说："+data);
}

void MainWindow::disConnect()
{
   tcpsocket->disconnectFromHost();
   tcpsocket->close();
   tcpsocket=NULL;
   ui->display_message->append("客户端与服务器端断开成功！");

   ui->connect_bt->setEnabled(true);
}

void MainWindow::sendMessage()
{

}

void MainWindow::on_send_message_clicked()
{
    //获取文本框的输入，开始发送消息
   QString str=ui->text_input->text().trimmed();

   if(str.isEmpty()){
       ui->statusbar->showMessage("不能发送空消息哦！");
   }else {
       tcpsocket->write(str.toUtf8().data());
       ui->display_message->append("我跟服务器端说："+str);

   }
}

