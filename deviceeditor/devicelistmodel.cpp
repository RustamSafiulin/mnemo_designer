#include "devicelistmodel.h"
#include <QIcon>

DeviceListModel::DeviceListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QVariant DeviceListModel::data(const QModelIndex &index, int role) const {

    QVariant result;

    if (!index.isValid()) {
        return result;
    }

    if (role == Qt::DisplayRole) {
        result = devices.at(index.row());
    }
    else if (role == Qt::DecorationRole) {
        result = QIcon(":/images/deviceeditor.png");
    }

    return result;
}

Qt::ItemFlags DeviceListModel::flags(const QModelIndex &index) const {

    Q_UNUSED(index)

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

int DeviceListModel::rowCount(const QModelIndex &parent) const {

    Q_UNUSED(parent)
    return devices.size();
}

int DeviceListModel::columnCount(const QModelIndex &parent) const {

    Q_UNUSED(parent)
    return 1;
}

void DeviceListModel::addNewDeviceName(const QString &name) {

    beginInsertRows(QModelIndex(),devices.size(),devices.size());
    devices << name;
    endInsertRows();
}

void DeviceListModel::deleteDeviceName(const QString &name) {

    beginRemoveRows(QModelIndex(),devices.size(),devices.size());
    int index = devices.indexOf(name);

    if (index >= 0) {
        devices.removeAt(index);
    }

    endRemoveRows();
}

