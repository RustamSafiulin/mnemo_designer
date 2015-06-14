#include "signaltablemodel.h"
#include <QColor>

SignalTableModel::SignalTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    horizontalHeaders << trUtf8("Назначение") << trUtf8("Название")
                      << trUtf8("Краткое \n обозначение") << trUtf8("Код")
                      << trUtf8("Тип") << trUtf8("Вход/Выход");
}

QVariant SignalTableModel::data(const QModelIndex &index, int role) const {

    if (index.isValid() && role == Qt::DisplayRole) {
        return getData(index.row(),index.column());
    }
    else if (index.isValid() && role == Qt::BackgroundRole) {
        if (index.row() % 2 == 1) {
            return QColor(Qt::lightGray);
        }
    }
    return QVariant();
}

bool SignalTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {

    if (index.isValid() && role == Qt::EditRole) {
        Signal *signal = sigList.at(index.row());

        switch (index.column()) {

        case H_DEST:
            signal->setDestination(value.toString());
            break;
        case H_NAME:
            signal->setName(value.toString());
            break;
        case H_DESIGNATION:
            signal->setDesignation(value.toString());
            break;
        case H_CODE:
            signal->setCode(value.toString());
            break;
        case H_TYPE:
            signal->setType(value.toString());
            break;
        case H_DIRECT:
            signal->setDirection(value.toString());
            break;
        default:
            return false;
        }

        emit dataChanged(index,index);
        return true;
    }
    return false;
}

QVariant SignalTableModel::getData(int num, int pos) const {

    switch (pos) {
        case H_DEST:
            return QVariant(sigList.at(num)->getDestination());
        case H_NAME:
            return QVariant(sigList.at(num)->getName());
        case H_DESIGNATION:
            return QVariant(sigList.at(num)->getDesignation());
        case H_CODE:
            return QVariant(sigList.at(num)->getCode());
        case H_TYPE:
            return QVariant(sigList.at(num)->getType());
        case H_DIRECT:
            return QVariant(sigList.at(num)->getDirection());
    }

    return QVariant();
}

QVariant SignalTableModel::headerData(int section, Qt::Orientation orientation, int role) const {

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {

        switch (section) {

        case H_DEST:
            return horizontalHeaders[H_DEST];
        case H_NAME:
            return horizontalHeaders[H_NAME];
        case H_DESIGNATION:
            return horizontalHeaders[H_DESIGNATION];
        case H_CODE:
            return horizontalHeaders[H_CODE];
        case H_TYPE:
            return horizontalHeaders[H_TYPE];
        case H_DIRECT:
            return horizontalHeaders[H_DIRECT];
        }
    }
    else
        return QVariant();

    return QVariant();
}

Qt::ItemFlags SignalTableModel::flags(const QModelIndex &index) const {

    if (index.isValid()) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsEnabled
                | Qt::ItemIsSelectable;
    }

    return Qt::NoItemFlags;
}

int SignalTableModel::rowCount(const QModelIndex &parent) const {

    Q_UNUSED(parent)
    return sigList.size();
}

int SignalTableModel::columnCount(const QModelIndex &parent) const {

    Q_UNUSED(parent)
    return 6;
}

void SignalTableModel::addSignal(Signal *signal) {

    if (!signal) {
        return;
    }

    beginInsertRows(QModelIndex(),sigList.size(),sigList.size());
    sigList.append(signal);
    endInsertRows();
}

void SignalTableModel::deleteSignal(const QModelIndex &index) {

    if (index.isValid()) {
        beginRemoveRows(QModelIndex(),index.row(),index.row());
        sigList.removeAt(index.row());
        endRemoveRows();
    }
}

void SignalTableModel::resetModel() {

    beginResetModel();
    sigList.clear();
    endResetModel();
}
