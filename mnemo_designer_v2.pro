#-------------------------------------------------
#
# Project created by QtCreator 2014-11-05T12:28:07
#
#-------------------------------------------------

QT       += core xml

greaterThan(QT_MAJOR_VERSION, 4){
    QT += widgets printsupport
    CONFIG += c++11
}
else {
    QT += gui
    QMAKE_CXXFLAGS += -std=gnu++11
}

TARGET = mnemo_designer_v2
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    objectstreeview.cpp \
    projecttreeview.cpp \
    screen.cpp \
    elementlibrarydialog.cpp \
    elementsimplelistwidget.cpp \
    newcomponentdialog.cpp \
    undocommands.cpp \
    savestrategydialog.cpp \
    previewwindow.cpp \
    screenmanager.cpp \
    screenlinkdialog.cpp \
    xmlscreenconfigwriter.cpp \
    elementlibrarylistwidget.cpp \
    elementlibrarylistwidgetitem.cpp \

HEADERS  += mainwindow.h \
    objectstreeview.h \
    projecttreeview.h \
    screen.h \
    itemtypes.h \
    elementlibrarydialog.h \
    elementsimplelistwidget.h \
    newcomponentdialog.h \
    mnemodefs.h \
    undocommands.h \
    savestrategydialog.h \
    previewwindow.h \
    screenmanager.h \
    screenlinkdialog.h \
    xmlscreenconfigwriter.h \
    elementlibrarylistwidget.h \
    elementlibrarylistwidgetitem.h \

FORMS    += mainwindow.ui \
    elementlibrarydialog.ui \
    newcomponentdialog.ui \
    savestrategydialog.ui \
    previewwindow.ui \
    screenlinkdialog.ui \

RESOURCES += \
    images.qrc

RC_FILE = application.rc

include(graphicselements/graphicselements.pri)
include(propertyeditor/propertyeditor.pri)
include(deviceeditor/deviceeditor.pri)
include(indicationeditor/indicationeditor.pri)
