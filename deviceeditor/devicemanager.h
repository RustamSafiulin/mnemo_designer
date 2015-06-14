#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include "device.h"

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    static DeviceManager *getInstance();
    void addDevice(Device *device);
    bool removeDevice(Device *device);
    int getDevListSize() const;
    QList <Device*> getDevList() const;
    Device *getDeviceByName(const QString &name) const;

    void debugShow();
    bool saveConfig(const QString &filename);

private:
    DeviceManager(QObject *parent = 0);
    DeviceManager(const DeviceManager &);

    static DeviceManager *instance;
    QList <Device*> devList;
};

#endif // DEVICEMANAGER_H
