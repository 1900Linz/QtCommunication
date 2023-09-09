#pragma once
#include <QObject>
#include <QUuid>
#include <QSharedPointer>
#include <Windows.h>

class DevPluginInterface;
class DevPluginUSB : public QObject
{
    Q_OBJECT
public:
    explicit DevPluginUSB(QObject *parent = nullptr);
    ~DevPluginUSB();

    void init(const QVector<QUuid> &uuids);

    void free();

signals:
    // 设备插入
    void message(QString);

private:
    QSharedPointer<DevPluginInterface> dptr;
};
