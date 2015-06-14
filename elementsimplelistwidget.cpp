#include "elementsimplelistwidget.h"
#include <QMimeData>
#include <QDrag>
#include <QApplication>

ElementSimpleListWidget::ElementSimpleListWidget(QListWidget *parent) :
    QListWidget(parent)
{
    setSpacing(5);
    addElements();
}

void ElementSimpleListWidget::addElements() {

    QListWidgetItem *lineItem = new QListWidgetItem(QIcon(":/images/lineitem.png"),trUtf8("Линия"));
    QListWidgetItem *rectItem = new QListWidgetItem(QIcon(":/images/rectitem.png"),trUtf8("Прямоугольник"));
    QListWidgetItem *ellipseItem = new QListWidgetItem(QIcon(":/images/ellipseitem.png"),trUtf8("Эллипс"));
    QListWidgetItem *arrowItem = new QListWidgetItem(QIcon(":/images/arrowitem.png"),trUtf8("Стрелка"));
    QListWidgetItem *polygonItem = new QListWidgetItem(QIcon(":/images/polygon.png"),trUtf8("Полигон"));
    QListWidgetItem *textItem = new QListWidgetItem(QIcon(":/images/textitem.png"),trUtf8("Текст"));

    addItem(lineItem);
    addItem(rectItem);
    addItem(ellipseItem);
    addItem(arrowItem);
    addItem(polygonItem);
    addItem(textItem);
}

void ElementSimpleListWidget::mousePressEvent(QMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {
        startPos = event->pos();
    }

    QListWidget::mousePressEvent(event);
}

void ElementSimpleListWidget::mouseMoveEvent(QMouseEvent *event) {

    if (event->buttons() & Qt::LeftButton) {
         int distance = (event->pos() - startPos).manhattanLength();
         if (distance >= QApplication::startDragDistance())
             startDrag();
     }

     QListWidget::mouseMoveEvent(event);
}

void ElementSimpleListWidget::startDrag() {

    QListWidgetItem *item = currentItem();
    if (item) {
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(item->text());
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(QPixmap(currentItem()->icon().pixmap(50,50)));
        drag->start(Qt::MoveAction);
    }
}
