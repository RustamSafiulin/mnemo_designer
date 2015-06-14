#include "savestrategydialog.h"
#include <QtDebug>

SaveStrategyDialog::SaveStrategyDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    setWindowTitle(trUtf8("Сохранить экран как"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    listWidget->setCurrentRow(0);
    teDescription->setText(trUtf8("Сохранение экрана в формате XML c расширением .mdscreenxml"));

    connect(pbChoose,SIGNAL(clicked()),SLOT(slotChoose()));
    connect(pbCancel,SIGNAL(clicked()),SLOT(slotCancel()));
    connect(listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            SLOT(slotChooseDoubleClicked(QListWidgetItem*)));
    connect(listWidget,SIGNAL(itemClicked(QListWidgetItem*)),SLOT(slotItemClicked(QListWidgetItem*)));
}

SaveScreenType SaveStrategyDialog::getLastChoose() const {
    return lastChooseItem;
}

void SaveStrategyDialog::slotChoose() {

    if (listWidget->currentItem()->text().contains(trUtf8("XML"),Qt::CaseInsensitive)) {
        lastChooseItem = SAVE_XML;
        accept();
    }
    else if (listWidget->currentItem()->text().contains(trUtf8("Binary"),Qt::CaseInsensitive)) {
        lastChooseItem = SAVE_BINARY;
        accept();
    }
    else if (listWidget->currentItem()->text().contains(trUtf8("Изображение"),Qt::CaseInsensitive)) {
        lastChooseItem = SAVE_IMAGE;
        accept();
    }
    else {
        reject();
    }
}

void SaveStrategyDialog::slotCancel() {
    reject();
}

void SaveStrategyDialog::slotChooseDoubleClicked(QListWidgetItem *item) {

    Q_UNUSED(item)

    slotChoose();
}

void SaveStrategyDialog::slotItemClicked(QListWidgetItem *item) {

    if (item->text().contains(trUtf8("XML"),Qt::CaseInsensitive)) {
        teDescription->setText(trUtf8("Сохранение экрана в формате XML c расширением .mdscreenxml"));
    }
    else if (item->text().contains(trUtf8("Binary"),Qt::CaseInsensitive)) {
        teDescription->setText(trUtf8("Сохранение экрана в виде бинарного файла с расширением .mdscreenbin"));
    }
    else if (item->text().contains(trUtf8("Изображение"),Qt::CaseInsensitive)) {
        teDescription->setText(trUtf8("Сохранение экрана в виде растрового изображения"));
    }
}
