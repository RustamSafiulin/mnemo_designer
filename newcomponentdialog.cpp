#include "newcomponentdialog.h"

NewComponentDialog::NewComponentDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    setWindowTitle(trUtf8("Новый"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(pbChoose,SIGNAL(clicked()),SLOT(slotChoose()));
    connect(pbCancel,SIGNAL(clicked()),SLOT(slotCancel()));
    connect(listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            SLOT(slotChooseDoubleClicked(QListWidgetItem*)));
}

void NewComponentDialog::slotCancel() {
    reject();
}

void NewComponentDialog::slotChoose() {

    if (listWidget->currentItem()->text().contains(trUtf8("Новый экран"),Qt::CaseInsensitive)) {
        lastChooseItem = NEW_SCREEN;
    }

    accept();
}

ComponentType NewComponentDialog::getLastChoose() const {
    return lastChooseItem;
}

void NewComponentDialog::slotChooseDoubleClicked(QListWidgetItem *item) {

    Q_UNUSED(item)

    slotChoose();
}
