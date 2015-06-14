#ifndef ELEMENTARROW_H
#define ELEMENTARROW_H

#include <QGraphicsSceneEvent>
#include <QPainter>
#include <QDataStream>
#include "figure.h"

class ElementArrow : public Figure
{
    Q_OBJECT
public:
    ElementArrow();
    void setClickPosition(QPointF);
    void updateBoundingElement();
    void updateElementProperty(uint id, const QVariant &value);
    void updatePropertyModel();
    void createPropertyList();
    void writeAsXml(QXmlStreamWriter &);
    void readFromXml(const QXmlStreamAttributes &);

    enum {Type = ArrowItemType};
    int type() const {
        return Type;
    }

    friend QDataStream &operator<<(QDataStream &out,const ElementArrow &line);
    friend QDataStream &operator>>(QDataStream &in,ElementArrow &line);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    QLineF elementLine;
    TextProperty *idProperty;
    EmptyProperty *titleProperty;
    IntegerProperty *xCoordProperty;
    IntegerProperty *yCoordProperty;
    IntegerProperty *zValueProperty;
    IntegerProperty *widthProperty;
    IntegerProperty *heightProperty;
    ColorProperty *backColorProperty;
    IntegerProperty *borderWidthProperty;
    IntegerProperty *angleProperty;
    BoolProperty *blockedProperty;
    EmptyProperty *serviceProperty;
    TextProperty *screenLinkProperty;
    TextProperty *messageTypeProperty;

    qreal arrowSize;
};

#endif // ELEMENTARROW_H
