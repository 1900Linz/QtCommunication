#include "plugin.h"
#include <QMetaObject>
#include <QHash>
#include <QDebug>
#include <Dbt.h>
#pragma comment(lib, "user32.lib")

class DevPluginInterface
{
public:
    // 处理
    static LRESULT CALLBACK windowMessageProcess(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    static QString windowClassName();
    bool createMessageWindow(const QVector<QUuid> &uuids);
    void destroyMessageWindow();

    void transMessage(QString msg){
        QMetaObject::invokeMethod(ptr, "message", Qt::QueuedConnection,
                                  Q_ARG(QString, msg));
    }

    inline DEV_BROADCAST_DEVICEINTERFACE_W initDEVBrodcastInterface(){
        //
        DEV_BROADCAST_DEVICEINTERFACE_W filter_data;
        memset(&filter_data, 0, sizeof(DEV_BROADCAST_DEVICEINTERFACE_W));
        filter_data.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE_W);
        filter_data.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

        return filter_data;
    }

    // 关联的对象
    DevPluginUSB *ptr{nullptr};
    // 关联的窗口
    HWND hwnd{nullptr};
    // 设备通知句柄和 UUID/GUID
    QHash<QUuid, HDEVNOTIFY> devNotifys;
};

// 处理窗口消息
LRESULT CALLBACK DevPluginInterface::windowMessageProcess(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    QString device_name{};
    QString messageHead{};
    QString messageBody{};
    static QString unknownDevString{"unknownDev :"};
    static QString arrivalDevString{"Arrival Dev :"};
    static QString removeDevString{"Removed Dev :"};

    if (message != WM_DEVICECHANGE)
        return ::DefWindowProcW(hwnd, message, wParam, lParam);

    switch (wParam) {
    case DBT_DEVICEREMOVECOMPLETE: messageHead = removeDevString ;break;
    case DBT_DEVICEARRIVAL: messageHead = arrivalDevString ;break;
    default:  messageHead = unknownDevString;    break;
    }

    // 过滤 device interface class 以外类型的消息
    DEV_BROADCAST_HDR *broadcast = reinterpret_cast<DEV_BROADCAST_HDR *>(lParam);
    if (!broadcast || broadcast->dbch_devicetype != DBT_DEVTYP_DEVICEINTERFACE)
        return ::DefWindowProcW(hwnd, message, wParam, lParam);

    // 获取 SetWindowLongPtrW 设置的对象
    DevPluginInterface *data = reinterpret_cast<DevPluginInterface *>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (!data)
        return ::DefWindowProcW(hwnd, message, wParam, lParam);

    // 过滤不监听的设备类型
    DEV_BROADCAST_DEVICEINTERFACE *device_interface = reinterpret_cast<DEV_BROADCAST_DEVICEINTERFACE *>(broadcast);
    QUuid uid(device_interface->dbcc_classguid);
    if (!data->devNotifys.contains(uid))
        return ::DefWindowProcW(hwnd, message, wParam, lParam);

    //设备描述
    if (device_interface->dbcc_name) {
#ifdef UNICODE
        device_name = QString::fromWCharArray(device_interface->dbcc_name);
#else
        device_name = QString(device_interface->dbcc_name);
#endif

        messageBody = device_name.split("#")[1];
    }

    data->transMessage(messageHead + messageBody);
    return ::DefWindowProcW(hwnd, message, wParam, lParam);
}


QString DevPluginInterface::windowClassName()
{
    return QLatin1String("Qt_DeviceHotplug_Test_") + QString::number(quintptr(windowMessageProcess));
}


bool DevPluginInterface::createMessageWindow(const QVector<QUuid> &uuids)
{
    QString class_name = windowClassName();
    HINSTANCE hi = ::GetModuleHandleW(nullptr);

    WNDCLASSW wc;
    memset(&wc, 0, sizeof(WNDCLASSW));
    wc.lpfnWndProc = windowMessageProcess;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hi;
    wc.lpszClassName = reinterpret_cast<const wchar_t *>(class_name.utf16());
    ::RegisterClassW(&wc);

    hwnd = ::CreateWindowW(wc.lpszClassName, // classname
                           wc.lpszClassName, // window name
                           0,  // style
                           0,  // x
                           0,  // y
                           0,  // width
                           0,  // height
                           0,  // parent
                           0,  // menu handle
                           hi, // application
                           0); // windows creation data.
    if (!hwnd) {
        qDebug()<<"createMessageWindow error"<<(int)GetLastError();
    } else {
        DEV_BROADCAST_DEVICEINTERFACE_W filter_data = initDEVBrodcastInterface();
        for (auto &&uuid : uuids)
        {
            filter_data.dbcc_classguid = uuid;
            HDEVNOTIFY handle = ::RegisterDeviceNotificationW(hwnd, &filter_data, DEVICE_NOTIFY_WINDOW_HANDLE);
            if (handle) {
                devNotifys.insert(uuid, handle);
            } else {
                qDebug()<<"RegisterDeviceNotification error"<<uuid;
            }
        }
        ::SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)this);
    }
    return !!hwnd;
}

// 释放
void DevPluginInterface::destroyMessageWindow()
{
    if (hwnd) {
        ::DestroyWindow(hwnd);
        hwnd = nullptr;

        for (HDEVNOTIFY handle : qAsConst(devNotifys))
        {
            ::UnregisterDeviceNotification(handle);
        }
        devNotifys.clear();
    }
    ::UnregisterClassW(reinterpret_cast<const wchar_t *>(windowClassName().utf16()), ::GetModuleHandleW(nullptr));
}

DevPluginUSB::DevPluginUSB(QObject *parent)
    : QObject{parent}
    , dptr{new DevPluginInterface}
{
    dptr->ptr = this;
}

DevPluginUSB::~DevPluginUSB()
{
    free();
}

void DevPluginUSB::init(const QVector<QUuid> &uuids)
{
    const bool ret = dptr->createMessageWindow( uuids);
    if (!ret) {
        dptr->destroyMessageWindow();
    }
}

void DevPluginUSB::free()
{
    dptr->destroyMessageWindow();
}
