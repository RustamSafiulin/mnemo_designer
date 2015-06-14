#include "screenlinkdialog.h"
#include "screenmanager.h"
#include <QtDebug>

ScreenLinkDialog::ScreenLinkDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    setWindowFlags(Qt::Tool);
    setWindowTitle(trUtf8("Ссылка на экран"));

    connect(pbApply,SIGNAL(clicked()),SLOT(slotApply()));
}

void ScreenLinkDialog::fillContent() {

    QListIterator <Screen*> it(ScreenManager::getInstance()->getScreenList());

    while (it.hasNext()) {
        cbScreenLink->addItem(it.next()->getScreenId());
    }
}

QString ScreenLinkDialog::getObjectScreenLink() const {

    return cbScreenLink->currentText();
}

void ScreenLinkDialog::slotApply() {

    accept();
}

void ScreenLinkDialog::updateContent(const QString &elemId, const QString &screenLink) {

    setWindowTitle(trUtf8("Ссылка на экран [%1]").arg(elemId));
    cbScreenLink->setCurrentIndex(cbScreenLink->findText(screenLink));
}
