#include "projecttreeview.h"
#include "screen.h"
#include "screenmanager.h"

ProjectTreeView::ProjectTreeView(QWidget *parent)
    : QListWidget(parent)
{
    setIconSize(QSize(32,32));
    itemIcon = QIcon(":/images/screen.png");
}

void ProjectTreeView::addNewElement(const QString &id) {

    QListWidgetItem *item = new QListWidgetItem(itemIcon,id);
    addItem(item);
}

void ProjectTreeView::screenChangeName(int index, const QString &name) {

    if (item(index)) {
        item(index)->setText(name);
    }
}

void ProjectTreeView::updateView() {

    clear();

    QListIterator <Screen*> it(ScreenManager::getInstance()->getScreenList());

    while (it.hasNext()) {
        addNewElement(it.next()->getScreenId());
    }
}
