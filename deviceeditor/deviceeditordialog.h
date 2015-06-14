#ifndef DEVICEEDITORDIALOG_H
#define DEVICEEDITORDIALOG_H

#include <QDialog>
#include <QXmlStreamReader>
#include <QFile>
#include "signaltableview.h"
#include "signaltablemodel.h"
#include "device.h"
#include "devicelistmodel.h"
#include "ui_deviceeditordialog.h"

class DeviceEditorDialog : public QDialog, public Ui::DeviceEditorDialog
{
    Q_OBJECT
    
public:
    explicit DeviceEditorDialog(QWidget *parent = 0);
    bool getUnsavedFlag();

private:
    void createView();
    void createConnections();
    void updateUi();
    bool isUniqueName(const QString &name);
    void readDeviceConfig(QFile &file);
    void readDeviceTag(QXmlStreamReader &xml);
    void createDevice(QXmlStreamReader &xml);
    void createSignal(QXmlStreamReader &xml);

    SignalTableView *signalTableView;
    SignalTableModel *signalTableModel;
    DeviceListModel *deviceListModel;
    Device *currentDevice;
    bool unsaved;
    QString title;

public slots:
    void slotSave();

private slots:
    void slotOk();
    void slotAddNewDevice();
    void slotDeleteDevice();
    void slotAddNewSignal();
    void slotDeleteSignal();
    void slotExport();
    void slotDeviceChange(const QModelIndex &);
    void slotCloseAll();
};

#endif // DEVICEEDITORDIALOG_H
