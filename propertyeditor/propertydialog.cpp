#include "propertydialog.h"

PropertyDialog::PropertyDialog(const QString &title, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    setWindowFlags(Qt::Tool);
    setWindowTitle(title);
}

void PropertyDialog::setPropertyView(PropertyTableView *view) {
    horizontalLayout->addWidget(view);
}
