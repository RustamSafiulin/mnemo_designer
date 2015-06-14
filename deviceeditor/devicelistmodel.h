#ifndef DEVICETABLEMODEL_H
#define DEVICETABLEMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QObject>

class DeviceListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    DeviceListModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    void addNewDeviceName(const QString &);
    void deleteDeviceName(const QString &);

private:
    QStringList devices;
};

#endif // DEVICETABLEMODEL_H
