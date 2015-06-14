#include "signal.h"

Signal::Signal(QObject *parent) :
    QObject(parent)
{
}

void Signal::setDestination(const QString &destination) {

    this->destination = destination;
}

QString Signal::getDestination() const {

    return destination;
}

void Signal::setName(const QString &name) {

    this->name = name;
}

QString Signal::getName() const {

    return name;
}

void Signal::setDesignation(const QString &designation) {

    this->designation = designation;
}

QString Signal::getDesignation() const {

    return designation;
}

void Signal::setCode(const QString &code) {

    this->code = code;
}

QString Signal::getCode() const {

    return code;
}

void Signal::setType(const QString &type) {

    this->type = type;
}

QString Signal::getType() const {

    return type;
}

void Signal::setDirection(const QString &direction) {

    this->direction = direction;
}

QString Signal::getDirection() const {

    return direction;
}
