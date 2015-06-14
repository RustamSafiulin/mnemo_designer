#include "devicemanager.h"
#include <QXmlStreamWriter>
#include <QFile>
#include <QtDebug>

DeviceManager *DeviceManager::instance = 0;

DeviceManager::DeviceManager(QObject *parent) :
    QObject(parent)
{
}

DeviceManager *DeviceManager::getInstance() {

    if (instance == 0) {
        instance = new DeviceManager;
    }

    return instance;
}

void DeviceManager::addDevice(Device *device) {

    if (!device) {
        return;
    }

    devList.insert(devList.end(),device);
}

bool DeviceManager::removeDevice(Device *device) {

    int index = devList.indexOf(device);

    if (index >= 0) {
        devList.removeAt(index);
        return true;
    }

    return false;
}

int DeviceManager::getDevListSize() const {

    return devList.size();
}

QList <Device*> DeviceManager::getDevList() const {
    return devList;
}

bool DeviceManager::saveConfig(const QString &filename) {

    QFile file(filename);
    QXmlStreamWriter writer(&file);

    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    writer.setAutoFormatting(true);

    writer.writeStartDocument();
    writer.writeStartElement(trUtf8("Devices"));

    QListIterator <Device*> it(devList);

    while (it.hasNext()) {
        it.next()->saveAsXml(writer);
    }

    writer.writeEndElement();
    writer.writeEndDocument();

    return true;
}

Device *DeviceManager::getDeviceByName(const QString &name) const {

    Device *dev;

    foreach (dev, devList) {
        if (dev->getName() == name) {
            return dev;
        }
    }

    return 0;
}

void DeviceManager::debugShow() {

    foreach (Device *device,devList) {
        qDebug() << device->getName();
        qDebug() << "Signals--------------";

        foreach (Signal *sig,device->getSignalList()) {
            qDebug() << sig->getName();
        }

        qDebug() << "-SIgnals-------------\n";
    }
}
