#include "addsignaldialog.h"

AddSignalDialog::AddSignalDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    setWindowTitle(trUtf8("Новый сигнал"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(pbOk,SIGNAL(clicked()),SLOT(slotOk()));
    connect(pbCancel,SIGNAL(clicked()),SLOT(slotCancel()));
}

void AddSignalDialog::slotOk() {

    accept();
}

void AddSignalDialog::slotCancel() {

    reject();
}

QString AddSignalDialog::getDestination() const {
    return cbDestination->currentText();
}

QString AddSignalDialog::getName() const {
    return leName->text();
}

QString AddSignalDialog::getDesignation() const {
    return leDesignation->text();
}

QString AddSignalDialog::getCode() const {
    return leCode->text();
}

QString AddSignalDialog::getType() const {
    return cbType->currentText();
}

QString AddSignalDialog::getDirection() const {
    return cbDirection->currentText();
}


