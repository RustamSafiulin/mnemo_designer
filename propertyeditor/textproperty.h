#ifndef TEXTPROPERTY_H
#define TEXTPROPERTY_H

#include "property.h"

class TextProperty : public Property
{
public:
    TextProperty(const QString &name);

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& options, const QAbstractItemDelegate* delegate);
    virtual QVariant getEditorData(QWidget* editor) const;

protected:
    QString text;
};

#endif // TEXTPROPERTY_H
