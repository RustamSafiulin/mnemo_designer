#ifndef XMLSCREENCONFIGWRITER_H
#define XMLSCREENCONFIGWRITER_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QFile>
#include "screen.h"

class XmlScreenConfigWriter : QObject
{
public:
    XmlScreenConfigWriter(QObject *parent = 0);
    void writeScreenConfig(QFile &file, Screen *screen);
    void writeLibraryConfig(QFile &file,Screen *screen);
};

#endif // XMLSCREENCONFIGWRITER_H
