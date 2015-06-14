#ifndef ELEMENTLIBRARYDIALOG_H
#define ELEMENTLIBRARYDIALOG_H

#include <QDialog>
#include "elementsimplelistwidget.h"
#include "elementlibrarylistwidget.h"
#include "ui_elementlibrarydialog.h"

class ElementLibraryDialog : public QDialog, public Ui::ElementLibraryDialog
{
    Q_OBJECT
    
public:
    explicit ElementLibraryDialog(QWidget *parent = 0);

private:
    ElementSimpleListWidget simpleListWidget;
    ElementLibraryListWidget libraryListWidget;
};

#endif // ELEMENTLIBRARYDIALOG_H
