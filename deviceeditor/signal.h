#ifndef SIGNAL_H
#define SIGNAL_H

#include <QObject>

class Signal : public QObject
{
    Q_OBJECT
public:
    explicit Signal(QObject *parent = 0);
    
    void setDestination(const QString &destination);
    QString getDestination() const;

    void setName(const QString &name);
    QString getName() const;

    void setDesignation(const QString &designation);
    QString getDesignation() const;

    void setCode(const QString &code);
    QString getCode() const;

    void setType(const QString &type);
    QString getType() const;

    void setDirection(const QString &direction);
    QString getDirection() const;

private:
    QString destination;
    QString name;
    QString designation;
    QString code;
    QString type;
    QString direction;
};

#endif // SIGNAL_H
