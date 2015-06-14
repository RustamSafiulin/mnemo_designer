#include "signalitemdelegate.h"
#include <QComboBox>
#include <QLineEdit>
#include <QWidget>

SignalItemDelegate::SignalItemDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *SignalItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    if (index.isValid()) {
        switch (index.column()) {

        case 0: {
            QComboBox *editor = new QComboBox(parent);
            editor->addItem(trUtf8("Состояние"));
            editor->addItem(trUtf8("Расшифровка"));
            editor->addItem(trUtf8("Простая ком."));
            editor->addItem(trUtf8("Сложная ком."));
            editor->addItem(trUtf8("Показание датчика"));
            return editor;
            }
        case 1: {
            QLineEdit *editor = new QLineEdit(parent);
            return editor;
            }
        case 2: {
            QLineEdit *editor = new QLineEdit(parent);
            return editor;
            }
        case 3: {
            QLineEdit *editor = new QLineEdit(parent);
            return editor;
            }
        case 4: {
            QComboBox *editor = new QComboBox(parent);
            editor->addItem(trUtf8("Числовой (hex)"));
            editor->addItem(trUtf8("Числовой (dec)"));
            editor->addItem(trUtf8("Текстовый"));
            editor->addItem(trUtf8("Логический"));
            return editor;
            }
        case 5: {
            QComboBox *editor = new QComboBox(parent);
            editor->addItem(trUtf8("Входной"));
            editor->addItem(trUtf8("Выходной"));
            return editor;
            }
        }
    }

    return QItemDelegate::createEditor(parent,option,index);
}

void SignalItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {

    if (index.isValid() && editor) {

        if (index.column() == 0 || index.column() == 4 || index.column() == 5) {
            QString value = index.model()->data(index,Qt::DisplayRole).toString();
            QComboBox *combo = static_cast<QComboBox*>(editor);
            combo->setEditText(value);
        }
        else if (index.column() == 1 || index.column() == 2 || index.column() == 3)  {
            QString value = index.model()->data(index,Qt::DisplayRole).toString();
            QLineEdit *edit = static_cast<QLineEdit*>(editor);
            edit->setText(value);
        }
    }
    else {
        QItemDelegate::setEditorData(editor,index);
    }
}

void SignalItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {

    if (index.column() == 0 || index.column() == 4 || index.column() == 5) {
        QComboBox *combo = static_cast<QComboBox*>(editor);
        model->setData(index,combo->currentText(),Qt::EditRole);
    }
    else if (index.column() == 1 || index.column() == 2 || index.column() == 3) {
        QLineEdit *edit = static_cast<QLineEdit*>(editor);
        model->setData(index,edit->text(),Qt::EditRole);
    }
}
