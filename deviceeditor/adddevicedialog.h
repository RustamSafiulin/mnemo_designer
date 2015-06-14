#ifndef ADDDEVICEDIALOG_H
#define ADDDEVICEDIALOG_H

#include <QDialog>
#include "ui_adddevicedialog.h"

class AddDeviceDialog : public QDialog, public Ui::AddDeviceDialog
{
    Q_OBJECT
    
public:
    explicit AddDeviceDialog(QWidget *parent = 0);
    QString getDeviceName() const;

private slots:
    void slotOk();
    void slotCancel();
};

#endif // ADDDEVICEDIALOG_H
