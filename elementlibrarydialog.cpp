#include "elementlibrarydialog.h"

ElementLibraryDialog::ElementLibraryDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    simpleGridLayout->addWidget(&simpleListWidget);
    libraryGridLayout->addWidget(&libraryListWidget);
    setWindowFlags(Qt::Tool);

    connect(pbLoadLibrary,SIGNAL(clicked()),&libraryListWidget,SLOT(loadLibrary()));
}

