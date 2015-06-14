#include "deviceeditordialog.h"
#include "adddevicedialog.h"
#include "addsignaldialog.h"
#include "devicemanager.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QtDebug>

/**
Разобраться, когда будет время с утечками памяти при удалении объектов класса Device
*/

DeviceEditorDialog::DeviceEditorDialog(QWidget *parent) :
    QDialog(parent),currentDevice(0), unsaved(false),title(trUtf8("Редактор устройств и сигналов"))
{
    setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    createView();
    createConnections();
    updateUi();
}

void DeviceEditorDialog::createConnections() {

    connect(pbOk,SIGNAL(clicked()),SLOT(slotOk()));
    connect(pbAddDevice,SIGNAL(clicked()),SLOT(slotAddNewDevice()));
    connect(pbDeleteDevice,SIGNAL(clicked()),SLOT(slotDeleteDevice()));
    connect(pbAddSignal,SIGNAL(clicked()),SLOT(slotAddNewSignal()));
    connect(pbDeleteSignal,SIGNAL(clicked()),SLOT(slotDeleteSignal()));
    connect(pbExport,SIGNAL(clicked()),SLOT(slotExport()));
    connect(pbSave,SIGNAL(clicked()),SLOT(slotSave()));
    connect(pbCloseAll,SIGNAL(clicked()),SLOT(slotCloseAll()));
    connect(devListView,SIGNAL(clicked(const QModelIndex &)),SLOT(slotDeviceChange(const QModelIndex &)));
}

void DeviceEditorDialog::createView() {

    signalTableModel = new SignalTableModel;
    signalTableView = new SignalTableView(signalTableModel,this);
    horizontalLayout->addWidget(signalTableView);

    pbSave->setIcon(QIcon(":/images/filesave.png"));
    pbAddDevice->setIcon(QIcon(":/images/add.png"));
    pbDeleteDevice->setIcon(QIcon(":/images/delete.png"));
    pbAddSignal->setIcon(QIcon(":/images/add.png"));
    pbDeleteSignal->setIcon(QIcon(":/images/delete.png"));
    pbOk->setIcon(QIcon(":/images/apply.png"));
    pbExport->setIcon(QIcon(":/images/export.png"));

    deviceListModel = new DeviceListModel;
    devListView->setModel(deviceListModel);
    devListView->setIconSize(QSize(32,32));
}

void DeviceEditorDialog::updateUi() {

    DeviceManager::getInstance()->getDevListSize() ? signalTableView->setEnabled(true) :
                                                          signalTableView->setEnabled(false);
    DeviceManager::getInstance()->getDevListSize() ? pbAddSignal->setEnabled(true) :
                                                          pbAddSignal->setEnabled(false);
    DeviceManager::getInstance()->getDevListSize() ? pbDeleteSignal->setEnabled(true) :
                                                          pbDeleteSignal->setEnabled(false);
    DeviceManager::getInstance()->getDevListSize() ? pbExport->setEnabled(false) :
                                                          pbExport->setEnabled(true);
    DeviceManager::getInstance()->getDevListSize() ? pbSave->setEnabled(true) :
                                                          pbSave->setEnabled(false);

    devListView->selectionModel()->selectedIndexes().size() ? pbDeleteDevice->setEnabled(true) :
                                                              pbDeleteDevice->setEnabled(false);

    if (currentDevice) {
        lbCurrentDevice->setText(currentDevice->getName());
    }
    else {
        lbCurrentDevice->setText("");
    }

    if (unsaved) {
        setWindowModified(true);
        setWindowTitle(QString("%1[*]").arg(title));
    }
    else {
        setWindowModified(false);
        setWindowTitle(QString("%1[*]").arg(title));
    }
}

void DeviceEditorDialog::slotOk() {

    accept();
}

bool DeviceEditorDialog::isUniqueName(const QString &name) {

    foreach (Device *dev, DeviceManager::getInstance()->getDevList()) {
        if (name == dev->getName()) {
            return false;
        }
    }

    return true;
}

void DeviceEditorDialog::slotAddNewDevice() {

    AddDeviceDialog devDialog(this);
    devDialog.exec();

    if (devDialog.result() == QDialog::Accepted) {

        QString name = devDialog.getDeviceName();

        if (!isUniqueName(name)) {
            QMessageBox::information(this,trUtf8("Ошибка создания устройства"),trUtf8("Имя устройства должно быть уникальным!"), QMessageBox::Ok);
            return;
        }

        deviceListModel->addNewDeviceName(name);
        Device *device = new Device;
        device->setName(name);
        DeviceManager::getInstance()->addDevice(device);

        currentDevice = device;
        unsaved = true;
        updateUi();
    }
}

void DeviceEditorDialog::slotDeleteDevice() {

    if (!currentDevice) {
        return;
    }

    if (!DeviceManager::getInstance()->removeDevice(currentDevice)) {
        return;
    }

    deviceListModel->deleteDeviceName(currentDevice->getName());
    signalTableModel->resetModel();

    /*delete currentDevice;
       Crash program, mistake (undefined behavior)
    */

    if (DeviceManager::getInstance()->getDevList().isEmpty()) {
        return;
    }

    currentDevice = DeviceManager::getInstance()->getDevList().first();

    foreach (Signal *sig,currentDevice->getSignalList()) {
        signalTableModel->addSignal(sig);
    }

    unsaved = true;
    updateUi();
}

void DeviceEditorDialog::slotAddNewSignal() {

    AddSignalDialog sigDialog(this);
    sigDialog.exec();

    if (sigDialog.result() == QDialog::Accepted) {

        Signal *signal = new Signal;
        signal->setDestination(sigDialog.getDestination());
        signal->setName(sigDialog.getName());
        signal->setDesignation(sigDialog.getDesignation());
        signal->setCode(sigDialog.getCode());
        signal->setType(sigDialog.getType());
        signal->setDirection(sigDialog.getDirection());

        if (currentDevice) {
            currentDevice->addSignal(signal);
        }

        signalTableModel->resetModel();

        foreach(Signal *sig,currentDevice->getSignalList()) {
            signalTableModel->addSignal(sig);
        }
    }

    unsaved = true;
    updateUi();
}

void DeviceEditorDialog::slotDeviceChange(const QModelIndex &index) {

    Device *device = DeviceManager::getInstance()->getDevList().at(index.row());
    currentDevice = device;

    if (device) {

        signalTableModel->resetModel();

        foreach(Signal *sig,device->getSignalList()) {
            signalTableModel->addSignal(sig);
        }
    }

    updateUi();
}

void DeviceEditorDialog::slotDeleteSignal() {

    int elemNum = signalTableView->selectionModel()->currentIndex().row();
    signalTableModel->deleteSignal(signalTableView->selectionModel()->currentIndex());
    currentDevice->deleteSignal(elemNum);
    unsaved = true;
    updateUi();
}

void DeviceEditorDialog::slotCloseAll() {

    foreach (Device *dev,DeviceManager::getInstance()->getDevList()) {

        currentDevice = dev;
        slotDeleteDevice();
    }

    currentDevice = 0;
    updateUi();
}

void DeviceEditorDialog::slotSave() {

    QString filename = QFileDialog::getSaveFileName(this,trUtf8("Сохранение конфигурации устройств"),
                                                           ".",trUtf8("Mnemo device config (*.mddevconf)"));

    if (filename.isEmpty()) {
        return;
    }

    if (!filename.endsWith(".mddevconf")) {
        filename += ".mddevconf";
    }

    if (!DeviceManager::getInstance()->saveConfig(filename)) {
        QMessageBox::information(this,trUtf8("Ошибка сохранения"),
                                 trUtf8("Не удается сохранить файл, попробуйте еще раз"),QMessageBox::Ok);
    }
    else {
        QMessageBox::information(this,trUtf8("Успешное сохранение"),
                                 trUtf8("Файл конфигурации устройств успешно сохранен"),QMessageBox::Ok);
    }

    unsaved = false;
    updateUi();
}

void DeviceEditorDialog::slotExport() {

    QString filename = QFileDialog::getOpenFileName(this,trUtf8("Открытие файла конфигурации устройств"),
                                                    ".",trUtf8("*.mddevconf"));
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this,trUtf8("Ошибка"),trUtf8("Не удается открыть файл конфигурации устройств"),
                                 QMessageBox::Ok);
        return;
    }

    QXmlStreamReader reader(&file);
    readDeviceConfig(file);
    file.close();

    foreach(Signal *sig,currentDevice->getSignalList()) {
        signalTableModel->addSignal(sig);
    }


    updateUi();
}

void DeviceEditorDialog::readDeviceConfig(QFile &file) {

    QXmlStreamReader reader(&file);

    while (!reader.atEnd() && !reader.hasError()) {

        QXmlStreamReader::TokenType token = reader.readNext();

        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {

            if (reader.name() == "Devices") {
                continue;
            }

            if (reader.name() == "Device") {
                readDeviceTag(reader);
            }
        }
    }
}

void DeviceEditorDialog::readDeviceTag(QXmlStreamReader &xml) {

    createDevice(xml);
    xml.readNext();

    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Device")) {

        if (xml.tokenType() == QXmlStreamReader::StartElement) {

            if (xml.name() == "Signal") {
                createSignal(xml);
            }
        }

        xml.readNext();
    }
}

void DeviceEditorDialog::createDevice(QXmlStreamReader &xml) {

    Device *dev = new Device;
    currentDevice = dev;

    if (xml.attributes().hasAttribute("Name")) {
        dev->setName(xml.attributes().value("Name").toString());
    }

    DeviceManager::getInstance()->addDevice(dev);
    deviceListModel->addNewDeviceName(dev->getName());
}

void DeviceEditorDialog::createSignal(QXmlStreamReader &xml) {

    Signal *sig = new Signal;

    if (xml.attributes().hasAttribute("Destination")) {
        sig->setDestination(xml.attributes().value("Destination").toString());
    }

    if (xml.attributes().hasAttribute("Name")) {
        sig->setName(xml.attributes().value("Name").toString());
    }

    if (xml.attributes().hasAttribute("Designation")) {
        sig->setDesignation(xml.attributes().value("Designation").toString());
    }

    if (xml.attributes().hasAttribute("Code")) {
        sig->setCode(xml.attributes().value("Code").toString());
    }

    if (xml.attributes().hasAttribute("Type")) {
        sig->setType(xml.attributes().value("Type").toString());
    }

    if (xml.attributes().hasAttribute("Direction")) {
        sig->setDirection(xml.attributes().value("Direction").toString());
    }

    currentDevice->addSignal(sig);
}

bool DeviceEditorDialog::getUnsavedFlag() {
    return unsaved;
}
