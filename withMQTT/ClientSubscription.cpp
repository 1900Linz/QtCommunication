#include "ClientSubscription.h"
#include <QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QLoggingCategory>
ClientSubscription::ClientSubscription()
{
    m_pClient = nullptr;
}

ClientSubscription &ClientSubscription::getInstance()
{
    static ClientSubscription s_obj;
    return s_obj;
}

void ClientSubscription::init(QString sIp, quint16 nPort, QString sClientId, QString sUser, QString sPwd)
{
    if(m_pClient)
    {
        delete m_pClient;
        m_pClient = nullptr;
    }
    m_pClient = new QMQTT::Client(QHostAddress(sIp), nPort);
    if(m_pClient)
    {
        qDebug() << "m_pClient Create Success.";
        m_pClient->setClientId(sClientId);
        m_pClient->setUsername(sUser);
        m_pClient->setPassword(sPwd.toLatin1());
        m_pClient->setKeepAlive(60);

        connect(m_pClient, &QMQTT::Client::connected, this, &ClientSubscription::sig_result_connect);
        connect(m_pClient, &QMQTT::Client::disconnected, this, &ClientSubscription::sig_result_disconnect);
        connect(m_pClient, SIGNAL(received(QMQTT::Message)), this, SLOT(on_dataReceived(QMQTT::Message)));
    }
}

void ClientSubscription::slot_connect()
{
    qDebug() << "connect 000";
    if(m_pClient)
    {
        if(!m_pClient->isConnectedToHost())
        {
            qDebug() << "connect 111";
            m_pClient->connectToHost();
        }
    }
}

void ClientSubscription::slot_disconnect()
{
    qDebug() << "disconnect";
    if(m_pClient->isConnectedToHost())
    {
        m_pClient->disconnectFromHost();
    }
}

void ClientSubscription::slot_publish(QString sTopic, QString sPayload)
{
    if(sTopic.isEmpty() || sPayload.isEmpty())
    {
        qDebug() << "sTopic or sPayload is empty";
        return;
    }
    QMQTT::Message message(136, sTopic, sPayload.toUtf8());
    m_pClient->publish(message);
}

void ClientSubscription::slot_subscribe(QString sTopic)
{
    if(sTopic.isEmpty())
    {
        qDebug() << "sTopic is empty";
        return;
    }
    m_pClient->subscribe(sTopic);
}

void ClientSubscription::slot_unSubscribe(QString sTopic)
{
    if(sTopic.isEmpty())
    {
        qDebug() << "sTopic is empty";
        return;
    }
    m_pClient->unsubscribe(sTopic);
}

void ClientSubscription::on_dataReceived(QMQTT::Message message)
{
    qDebug() << "on_dataReceived";
    emit sig_result_dataReceived(message);
}
