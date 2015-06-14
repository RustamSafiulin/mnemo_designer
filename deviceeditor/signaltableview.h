#ifndef SIGNALTABLEVIEW_H
#define SIGNALTABLEVIEW_H

#include <QTableView>
#include <QHeaderView>
#include "signaltablemodel.h"
#include "signalitemdelegate.h"

class SignalTableView : public QTableView
{
    Q_OBJECT
public:
    SignalTableView(SignalTableModel *model,QWidget *parent = 0);

private:
    SignalItemDelegate *delegate;

    void init();
};

#endif // SIGNALTABLEVIEW_H
