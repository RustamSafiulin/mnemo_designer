#include "xmlscreenconfigwriter.h"

XmlScreenConfigWriter::XmlScreenConfigWriter(QObject *parent)
    : QObject(parent)
{
}

void XmlScreenConfigWriter::writeScreenConfig(QFile &file, Screen *screen) {

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("screen");
    writer.writeAttribute("filename",screen->getFileName());
    writer.writeAttribute("screenId",screen->getScreenId());
    writer.writeAttribute("priority",screen->getScreenPriority());
    writer.writeAttribute("width",QString::number(screen->getScreenWidth()));
    writer.writeAttribute("height",QString::number(screen->getScreenHeight()));
    writer.writeAttribute("background",screen->getScreenBackground().name());

    QListIterator <QGraphicsItem*> it(screen->items());

    while (it.hasNext()) {

        Figure *fig = static_cast<Figure*>(it.next());
        fig->writeAsXml(writer);
    }

    writer.writeEndElement();
    writer.writeEndDocument();
}

void XmlScreenConfigWriter::writeLibraryConfig(QFile &file, Screen *screen) {

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("Library");

    QListIterator <QGraphicsItem*> it(screen->selectedItems());

    while (it.hasNext()) {

        Figure *fig = static_cast<Figure*>(it.next());
        fig->writeAsXml(writer);
    }

    writer.writeEndElement();
    writer.writeEndDocument();
}
