#include "elementtext.h"
#include <QtDebug>

ElementText::ElementText()
{
    elementId = trUtf8("Текст");
    internalElementType = trUtf8("Text");
    elementIcon = QIcon(":/images/textitem.png");

    init();
    createPropertyList();
    updatePropertyModel();
}

QRectF ElementText::boundingRect() const {

    qreal extra = 5;

    QRectF rect(elementRect.toRect());
    return rect.normalized().adjusted(-extra,-extra,extra,extra);
}

QPainterPath ElementText::shape() const {
    QPainterPath path;
    path.addRect(elementRect);

    if (isSelected()) {
        path.addRect(QRectF(elementRect.topLeft() - QPointF(3,3),elementRect.topLeft() + QPointF(3,3)));
        path.addRect(QRectF(elementRect.bottomRight() - QPointF(3,3),elementRect.bottomRight() + QPointF(3,3)));
    }

    return path;
}

void ElementText::createPropertyList() {

    idProperty = new TextProperty(trUtf8("Идентификатор"));
    idProperty->setId(EL_ID);
    propList.insert(propList.end(),idProperty);

    titleProperty = new EmptyProperty(trUtf8("Внешний вид"));
    propList.insert(propList.end(),titleProperty);

    xCoordProperty = new IntegerProperty(trUtf8("Координата X"));
    xCoordProperty->setSettings(0,5000);
    xCoordProperty->setId(EL_X);
    propList.insert(propList.end(),xCoordProperty);

    yCoordProperty = new IntegerProperty(trUtf8("Координата Y"));
    yCoordProperty->setId(EL_Y);
    yCoordProperty->setSettings(0,5000);
    propList.insert(propList.end(),yCoordProperty);

    zValueProperty = new IntegerProperty(trUtf8("Z значение"));
    zValueProperty->setId(EL_Z_VALUE);
    zValueProperty->setSettings(-1000,1000);
    propList.insert(propList.end(),zValueProperty);

    widthProperty = new IntegerProperty(trUtf8("Ширина"));
    widthProperty->setId(EL_WIDTH);
    widthProperty->setSettings(0,5000);
    propList.insert(propList.end(),widthProperty);

    heightProperty = new IntegerProperty(trUtf8("Высота"));
    heightProperty->setId(EL_HEIGHT);
    heightProperty->setSettings(0,5000);
    propList.insert(propList.end(),heightProperty);

    elementTextProperty = new TextProperty(trUtf8("Текст"));
    elementTextProperty->setId(EL_TEXT);
    propList.insert(propList.end(),elementTextProperty);

    textColorProperty = new ColorProperty(trUtf8("Цвет текста"));
    textColorProperty->setId(EL_FONT_COLOR);
    propList.insert(propList.end(),textColorProperty);

    fontSizeProperty = new IntegerProperty(trUtf8("Размер текста"));
    fontSizeProperty->setId(EL_FONT_SIZE);
    fontSizeProperty->setSettings(8,72);
    propList.insert(propList.end(),fontSizeProperty);

    angleProperty = new IntegerProperty(trUtf8("Угол поворота"));
    angleProperty->setId(EL_ANGLE);
    angleProperty->setSettings(0,360);
    propList.insert(propList.end(),angleProperty);

    serviceProperty = new EmptyProperty(trUtf8("Служебное"));
    propList.insert(propList.end(),serviceProperty);

    blockedProperty = new BoolProperty(trUtf8("Блокировать"));
    blockedProperty->setId(EL_BLOCK);
    propList.insert(propList.end(),blockedProperty);

    screenLinkProperty = new TextProperty(trUtf8("Ссылка на экран"));
    screenLinkProperty->setId(EL_SCREEN_LINK);
    propList.insert(propList.end(),screenLinkProperty);

    messageTypeProperty = new TextProperty(trUtf8("Тип сообщения"));
    messageTypeProperty->setId(EL_MESSAGE_TYPE);
    propList.insert(propList.end(),messageTypeProperty);
}

void ElementText::updateElementProperty(uint id, const QVariant &value) {

    switch (id) {

    case EL_ID:
        elementId = value.toString();
        break;
    case EL_X:
        elementXPos = value.toInt();
        setElementXPos(elementXPos);
        break;
    case EL_Y:
        elementYPos = value.toInt();
        setElementYPos(elementYPos);
        break;
    case EL_Z_VALUE:
        elementZValue = value.toInt();
        setZValue(elementZValue);
        break;
    case EL_WIDTH:
        elementWidth = value.toInt();
        updateBoundingElement();
        break;
    case EL_HEIGHT:
        elementHeight = value.toInt();
        updateBoundingElement();
        break;
    case EL_FONT_COLOR:
        textColor = value.value<QColor>();
        break;
    case EL_FONT_SIZE:
        fontSize = value.toInt();
        break;
    case EL_TEXT:
        elementText = value.toString();
        break;
    case EL_ANGLE:
        elemAngle = value.toInt();
        setAngle(elemAngle);
        break;
    case EL_BLOCK:
        block = value.toBool();
        setBlocked(block);
        break;
    case EL_SCREEN_LINK:
        screenLink = value.toString();
        setScreenLink(screenLink);
        break;
    case EL_MESSAGE_TYPE:
        messageType = value.toString();
        setMessageType(messageType);
        break;
    }

    update();
    scene()->update();
}

void ElementText::updatePropertyModel() {

    idProperty->setValue(elementId);
    xCoordProperty->setValue(elementXPos);
    yCoordProperty->setValue(elementYPos);
    zValueProperty->setValue(elementZValue);
    widthProperty->setValue(elementWidth);
    heightProperty->setValue(elementHeight);
    textColorProperty->setValue(textColor);
    fontSizeProperty->setValue(fontSize);
    elementTextProperty->setValue(elementText);
    angleProperty->setValue(elemAngle);
    blockedProperty->setValue(block);
    screenLinkProperty->setValue(screenLink);
    messageTypeProperty->setValue(messageType);
}

void ElementText::setClickPosition(QPointF position) {

    prepareGeometryChange();
    elementXPos = position.x();
    elementYPos = position.y();
    setX(elementXPos);
    setY(elementYPos);

    elementRect.setRect(0,0,elementWidth,elementHeight);
    updatePropertyModel();
}

void ElementText::updateBoundingElement() {
    elementRect.setRect(0,0,elementWidth,elementHeight);
}

void ElementText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing);

    drawText(painter);

    if (isSelected()) {

        painter->setPen(QPen(borderColor));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect());

        setCursor(Qt::SizeAllCursor);
        painter->setBrush(Qt::red);
        painter->setPen(Qt::red);
        painter->drawRect(QRectF(elementRect.topLeft() - QPointF(3,3),elementRect.topLeft() + QPointF(3,3)));
        painter->drawRect(QRectF(elementRect.bottomRight() - QPointF(3,3),elementRect.bottomRight() + QPointF(3,3)));
    }
}

void ElementText::drawText(QPainter *painter) {

    painter->setPen(textColor);
    painter->setBrush(Qt::NoBrush);

    QFont font = painter->font();
    font.setFamily("Arial Black");
    font.setPointSize(fontSize);

    painter->setFont(font);
    painter->drawText(boundingRect(),Qt::AlignCenter,elementText);
}

void ElementText::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

    QPointF mousePoint = event->pos();

    if (resizing) {

        setCursor(Qt::SizeFDiagCursor);

        switch (rd) {

        case RdBottomRight:
            elementRect.setBottomRight(mousePoint);
            elementWidth = qAbs(elementRect.topLeft().x() - elementRect.bottomRight().x());
            elementHeight = qAbs(elementRect.topLeft().y() - elementRect.bottomRight().y());
            break;
        case RdTopLeft:
            elementRect.setTopLeft(mousePoint);
            setElementXPos(mapToScene(elementRect.topLeft()).x());
            setElementYPos(mapToScene(elementRect.topLeft()).y());
            setElementWidth(qAbs(mapToScene(elementRect.topLeft()).x() - mapToScene(elementRect.bottomRight()).x()));
            setElementHeight(qAbs(mapToScene(elementRect.topLeft()).y() - mapToScene(elementRect.bottomRight()).y()));
            updateBoundingElement();
            break;
        case RdNone:
            QGraphicsObject::mouseMoveEvent(event);
            break;
        }

        scene()->update();
        return;
    }
    else {
        QGraphicsObject::mouseMoveEvent(event);
    }
}

void ElementText::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF topLeft = elementRect.topLeft();
    QPointF bottomRight = elementRect.bottomRight();

    if (mousePoint.x() <= (topLeft.x() + mouseHandler.x()) &&
        mousePoint.x() >= (topLeft.x() - mouseHandler.x()) &&
        mousePoint.y() <= (topLeft.y() + mouseHandler.y()) &&
        mousePoint.y() >= (topLeft.y() - mouseHandler.y()))
    {
        rd = RdTopLeft;
        resizing = true;
        setCursor(Qt::SizeFDiagCursor);
    }
    else if (mousePoint.x() <= (bottomRight.x() + mouseHandler.x()) &&
             mousePoint.x() >= (bottomRight.x() - mouseHandler.x()) &&
             mousePoint.y() <= (bottomRight.y() + mouseHandler.y()) &&
             mousePoint.y() >= (bottomRight.y() - mouseHandler.y()))
    {
        rd = RdBottomRight;
        resizing = true;
        setCursor(Qt::SizeFDiagCursor);
    }
    else {
        resizing = false;
        rd = RdNone;
    }

    oldPos = pos();
    oldWidth = elementWidth;
    oldHeight = elementHeight;

    QGraphicsObject::mousePressEvent(event);
}

void ElementText::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    setCursor(Qt::ArrowCursor);
    elementXPos = pos().x();
    elementYPos = pos().y();
    updatePropertyModel();

    if (oldPos != pos()) {
        emit elementMoved(oldPos);
    }

    if (resizing) {
        emit elementResized(oldWidth,oldHeight,oldPos);
    }

    QGraphicsObject::mouseReleaseEvent(event);
}

void ElementText::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {

    QPointF mousePoint = event->pos();
    QPointF mouseHandler = QPointF(3,3);
    QPointF topLeft = elementRect.topLeft();
    QPointF bottomRight = elementRect.bottomRight();

    if (mousePoint.x() <= (topLeft.x() + mouseHandler.x()) &&
        mousePoint.x() >= (topLeft.x() - mouseHandler.x()) &&
        mousePoint.y() <= (topLeft.y() + mouseHandler.y()) &&
        mousePoint.y() >= (topLeft.y() - mouseHandler.y()))
    {

        setCursor(Qt::SizeFDiagCursor);
    }
    else if (mousePoint.x() <= (bottomRight.x() + mouseHandler.x()) &&
             mousePoint.x() >= (bottomRight.x() - mouseHandler.x()) &&
             mousePoint.y() <= (bottomRight.y() + mouseHandler.y()) &&
             mousePoint.y() >= (bottomRight.y() - mouseHandler.y()))
    {

        setCursor(Qt::SizeFDiagCursor);
    }

    QGraphicsObject::hoverEnterEvent(event);
}

void ElementText::writeAsXml(QXmlStreamWriter &writer) {

    writer.writeStartElement("element");
    writer.writeAttribute("internalType",internalElementType);
    writer.writeAttribute("elementId",elementId);
    writer.writeAttribute("x",QString::number(x()));
    writer.writeAttribute("y",QString::number(y()));
    writer.writeAttribute("z",QString::number(zValue()));
    writer.writeAttribute("width",QString::number(elementWidth));
    writer.writeAttribute("height",QString::number(elementHeight));
    writer.writeAttribute("elementtext",elementText);
    writer.writeAttribute("textcolor",textColor.name());
    writer.writeAttribute("fontsize",QString::number(fontSize));
    writer.writeAttribute("elemAngle",QString::number(elemAngle));
    writer.writeAttribute("block",QString(QVariant(block).toString()));
    writer.writeAttribute("indicationrule",indicationRule);
    writer.writeAttribute("linkingType",linkingType);
    writer.writeAttribute("deviceLink",deviceLink);
    writer.writeAttribute("signalLink",signalLink);
    writer.writeAttribute("screenLink",screenLink);
    writer.writeAttribute("messageType",messageType);
    writer.writeEndElement();
}

void ElementText::readFromXml(const QXmlStreamAttributes &attributes) {

    if (attributes.hasAttribute("elementId")) {
        setElementId(attributes.value("elementId").toString());
    }

    if (attributes.hasAttribute("x")) {
        setElementXPos(attributes.value("x").toString().toInt());
    }

    if (attributes.hasAttribute("y")) {
        setElementYPos(attributes.value("y").toString().toInt());
    }

    if (attributes.hasAttribute("z")) {
        setZValue(attributes.value("z").toString().toInt());
    }

    if (attributes.hasAttribute("width")) {
        setElementWidth(attributes.value("width").toString().toInt());
    }

    if (attributes.hasAttribute("height")) {
        setElementHeight(attributes.value("height").toString().toInt());
    }

    if (attributes.hasAttribute("elementtext")) {
        elementText = attributes.value("elementtext").toString();
    }

    if (attributes.hasAttribute("textcolor")) {
        textColor = QColor(attributes.value("textcolor").toString());
    }

    if (attributes.hasAttribute("fontsize")) {
        fontSize = attributes.value("fontsize").toString().toInt();
    }

    if (attributes.hasAttribute("elemAngle")) {
        setAngle(attributes.value("elemAngle").toString().toInt());
    }

    if (attributes.hasAttribute("block")) {
        setBlocked(attributes.value("block").toString().toInt());
    }

    if (attributes.hasAttribute("indicationrule")) {
        setIndicationRule(attributes.value("indicationrule").toString());
    }

    if (attributes.hasAttribute("linkingType")) {
        setLinkingType(attributes.value("linkingType").toString());
    }

    if (attributes.hasAttribute("deviceLink")) {
        setDeviceLink(attributes.value("deviceLink").toString());
    }

    if (attributes.hasAttribute("signalLink")) {
        setSignalLink(attributes.value("signalLink").toString());
    }

    if (attributes.hasAttribute("screenLink")) {
        setScreenLink(attributes.value("screenLink").toString());
    }

    if (attributes.hasAttribute("messageType")) {
        setMessageType(attributes.value("messageType").toString());
    }

    updateBoundingElement();
    updatePropertyModel();
}

/** Дополнить сериализацию*/
QDataStream &operator<<(QDataStream &out,const ElementText &rect) {

    out << rect.elementId << rect.x() << rect.y()
        << rect.zValue() << rect.elementWidth << rect.elementHeight
        << rect.elementText << rect.textColor << rect.fontSize
        << rect.elemAngle << rect.block << rect.indicationRule;
    return out;
}

QDataStream &operator>>(QDataStream &in,ElementText &rect) {

    QString id;
    qreal xpos;
    qreal ypos;
    qreal zvalue;
    int width;
    int height;
    QString text;
    QColor textColor;
    int fontSize;
    qreal angle;
    bool block;
    QString rule;

    in >> id >> xpos >> ypos >> zvalue >> width >>
          height >> text >> textColor >> fontSize >> angle >> block >> rule;

    rect.setElementId(id);
    rect.setElementXPos(xpos);
    rect.setElementYPos(ypos);
    rect.setElementZValue(zvalue);
    rect.setElementWidth(width);
    rect.setElementHeight(height);
    rect.elementText = text;
    rect.textColor = textColor;
    rect.fontSize = fontSize;
    rect.setAngle(angle);
    rect.block = block;
    rect.setIndicationRule(rule);
    rect.updateBoundingElement();
    rect.updatePropertyModel();

    return in;
}

