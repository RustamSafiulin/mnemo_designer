#include "signaltableview.h"

SignalTableView::SignalTableView(SignalTableModel *model,QWidget *parent)
    : QTableView(parent)
{
    if (model) {
        setModel(model);
    }

    init();
}

void SignalTableView::init() {

    setColumnWidth(0,150);
    setSelectionMode(QTableView::SingleSelection);
    setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked);
    setAlternatingRowColors(true);
    verticalHeader()->hide();
    verticalHeader()->setDefaultSectionSize(25);
    horizontalHeader()->setStretchLastSection(true);

    delegate = new SignalItemDelegate(this);
    setItemDelegate(delegate);
}
