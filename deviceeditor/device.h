#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QXmlStreamWriter>
#include "signal.h"

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = 0);
    ~Device();

    void setName(const QString &name);
    QString getName() const; 

    void addSignal(Signal *signal);
    void deleteSignal(int index);
    void saveAsXml(QXmlStreamWriter &xml);

    QList <Signal*> getSignalList() const;
    QList <Signal*> getInputSignalList() const;

private:
    QString name;
    QList <Signal*> signalList;
    QList <Signal*> inputSignalList; //добавляются сигналы, имеющие направленность "Входной"
};

#endif // DEVICE_H
