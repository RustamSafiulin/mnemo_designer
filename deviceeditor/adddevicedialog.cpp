#include "adddevicedialog.h"

AddDeviceDialog::AddDeviceDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    setWindowTitle(trUtf8("Новое устройство"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(pbOk,SIGNAL(clicked()),SLOT(slotOk()));
    connect(pbCancel,SIGNAL(clicked()),SLOT(slotCancel()));
}

void AddDeviceDialog::slotOk() {

    accept();
}

void AddDeviceDialog::slotCancel() {

    reject();
}

QString AddDeviceDialog::getDeviceName() const {

    return leName->text();
}
