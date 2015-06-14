#ifndef SIGNALTABLEMODEL_H
#define SIGNALTABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include "signal.h"

class SignalTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SignalTableModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    void resetModel();
    void addSignal(Signal *signal);
    void deleteSignal(const QModelIndex &index);

private:
    enum {
        H_DEST,
        H_NAME,
        H_DESIGNATION,
        H_CODE,
        H_TYPE,
        H_DIRECT
    };

    QVariant getData(int num,int pos) const;
    QStringList horizontalHeaders;
    QList <Signal*> sigList;
};

#endif // SIGNALTABLEMODEL_H
