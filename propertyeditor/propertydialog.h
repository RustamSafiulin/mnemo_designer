#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include <QDialog>
#include "propertytableview.h"
#include "ui_propertydialog.h"

class PropertyDialog : public QDialog, public Ui::PropertyDialog
{
    Q_OBJECT
    
public:
    explicit PropertyDialog(const QString &title,QWidget *parent = 0);

    void setPropertyView(PropertyTableView *view);
};

#endif // PROPERTYDIALOG_H
