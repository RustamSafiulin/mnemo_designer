#include "device.h"

Device::Device(QObject *parent) :
    QObject(parent)
{
}

Device::~Device() {

    qDeleteAll(signalList);
}

void Device::setName(const QString &name) {

    this->name = name;
}

QString Device::getName() const {

    return name;
}


void Device::addSignal(Signal *signal) {

    if (!signal) {
        return;
    }

    if (signal->getDirection() == trUtf8("Входной")) {
        inputSignalList.insert(inputSignalList.end(),signal);
    }

    signalList.insert(signalList.end(),signal);
}

void Device::deleteSignal(int index) {

    Signal *sig = signalList.takeAt(index);
    inputSignalList.removeAt(index);
    delete sig;
}

QList <Signal*> Device::getSignalList() const {

    return signalList;
}

QList <Signal*> Device::getInputSignalList() const {

    return inputSignalList;
}

void Device::saveAsXml(QXmlStreamWriter &xml) {

    xml.writeStartElement(trUtf8("Device"));
    xml.writeAttribute(trUtf8("Name"),name);

    QListIterator <Signal*> it(signalList);

    while (it.hasNext()) {

        Signal *sig = it.next();
        xml.writeStartElement(trUtf8("Signal"));
        xml.writeAttribute(trUtf8("Destination"),sig->getDestination());
        xml.writeAttribute(trUtf8("Designation"),sig->getDesignation());
        xml.writeAttribute(trUtf8("Name"),sig->getName());
        xml.writeAttribute(trUtf8("Code"),sig->getCode());
        xml.writeAttribute(trUtf8("Type"),sig->getType());
        xml.writeAttribute(trUtf8("Direction"),sig->getDirection());
        xml.writeEndElement();
    }

    xml.writeEndElement();
}
