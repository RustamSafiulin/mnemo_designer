#ifndef SIGNALITEMDELEGATE_H
#define SIGNALITEMDELEGATE_H

#include <QItemDelegate>

class SignalItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit SignalItemDelegate(QObject *parent = 0);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // SIGNALITEMDELEGATE_H
