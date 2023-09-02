#ifndef MY_MQTT_CLIENT_H
#define MY_MQTT_CLIENT_H

#include <QtCore/QObject>
#include <QtQmqtt/qmqtt.h>
#include <QtWebSockets/QWebSocket>

class ClientSubscription : public QObject
{
    Q_OBJECT
public:
    ClientSubscription();

    static ClientSubscription& getInstance();

    void init(QString sIp, quint16 nPort, QString sClientId, QString sUser, QString sPwd);
private:
    QMQTT::Client *m_pClient;

signals:
    void sig_result_connect();
    void sig_result_disconnect();
    void sig_result_dataReceived(QMQTT::Message message);

public slots:
    void slot_connect();
    void slot_disconnect();
    void slot_publish(QString sTopic, QString sPayload);
    void slot_subscribe(QString sTopic);
    void slot_unSubscribe(QString sTopic);

    void on_dataReceived(QMQTT::Message message);

};
#endif //
