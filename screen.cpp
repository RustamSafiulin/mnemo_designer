#include "screen.h"
#include <cmath>
#include <QPainter>
#include <QMouseEvent>
#include <QFileInfo>
#include <QtDebug>
#include <QFileDialog>
#include "xmlscreenconfigwriter.h"

/** Template algorithms*/
template<template<typename T> class S, typename T>
T min(const S<T> &sequence)
{
    Q_ASSERT(!sequence.isEmpty());
    T minimum = sequence.first();
    foreach (const T &item, sequence)
        if (item < minimum)
            minimum = item;
    return minimum;
}

template<template<typename T> class S, typename T>
T max(const S<T> &sequence)
{
    Q_ASSERT(!sequence.isEmpty());
    T maximum = sequence.first();
    foreach (const T &item, sequence)
        if (item > maximum)
            maximum = item;
    return maximum;
}

const QString MimeType = "rti/mnemodesigner";
/** !!*/


Screen::Screen(const QRectF &rect, QObject *parent)
    : QGraphicsScene(parent),propertyModel(0), filename(QString()),unsavedFlag(false)
{

    setItemIndexMethod(QGraphicsScene::NoIndex);

    if (rect.width() == 0 || rect.height() == 0) {
        setSceneRect(0,0,1600,1200);
    }
    else {
        setSceneRect(rect);
    }

    gridVisible = false;

    screenWidth = sceneRect().width();
    screenHeight = sceneRect().height();
    screenPriority.clear();
    screenPriority.append(trUtf8("Главный"));
    screenBackground = QColor(Qt::black);

    m_undoStack = new QUndoStack(this);
    m_undoStack->setUndoLimit(20);

    createPropertyList();
    createContextMenuActions();
    updateActions();

    connect(this,SIGNAL(selectionChanged()),SLOT(slotSelectionChanged()));
}

void Screen::setActive(bool active) {

    onActive = active;
}

bool Screen::active() {
    return onActive;
}

bool Screen::getUnsavedFlag() {
    return unsavedFlag;
}

void Screen::setPropertyModel(PropertyModel *model) {
    propertyModel = model;
    fillScreenPropertyModel();

    connect(propertyModel,SIGNAL(onDataChangedByEditor(Property*)),SLOT(slotElementPropertyChanged(Property*)));
    connect(propertyModel,SIGNAL(onDataChangedByEditor(Property*)),SLOT(slotScreenPropertyChanged(Property*)));
}

void Screen::fillGridPixmap() {

    gridPixmap = QPixmap(QSize(sceneRect().width(),sceneRect().height()));
    gridPixmap.fill(screenBackground);

    QPainter painter(&gridPixmap);

    const int maxX = static_cast<int>(std::ceil(width())/gridSize)*gridSize;
    const int maxY = static_cast<int>(std::ceil(height())/gridSize)*gridSize;

    if (screenBackground == Qt::black) {
        painter.setPen(Qt::white);
        painter.setBrush(Qt::NoBrush);
    }
    else {
        painter.setPen(Qt::black);
        painter.setBrush(Qt::NoBrush);
    }

    for (int i = 0; i < maxX; i += gridSize) {
        for (int j = 0; j < maxY; j += gridSize) {
            painter.drawPoint(i,j);
        }
    }
}

void Screen::setGridVisible(bool on) {

    gridVisible = on;

    if (gridVisible) {
        fillGridPixmap();
    }
    else {
        gridPixmap = QPixmap(QSize(sceneRect().width(),sceneRect().height()));
        gridPixmap.fill(screenBackground);
    }

    invalidate(sceneRect(),BackgroundLayer);
}

bool Screen::isGridVisible() const {
    return gridVisible;
}

void Screen::slotScreenPropertyChanged(Property *property) {

    if (!selectedItems().isEmpty()) {
        return;
    }

    if (!active()) {
        return;
    }

    unsavedFlag = true;
    emit screenPropertyChanged();

    switch (property->getId()) {

    case SCREEN_ID:
        setScreenId(property->getValue().toString());
        break;
    case SCREEN_PRIORITY:
        setScreenPriority(property->getValue().toString());
        break;
    case SCREEN_BACKGROUND:
        setScreenBackground(property->getValue().value<QColor>());
        break;
    case SCREEN_WIDTH:
        setScreenWidth(property->getValue().toInt());
        break;
    case SCREEN_HEIGHT:
        setScreenHeight(property->getValue().toInt());
        break;
    }

    fillGridPixmap();
}

void Screen::cleanPropertyModel() {

    propertyModel->resetModel();
}

void Screen::fillScreenPropertyModel() {

    idProperty->setValue(screenId);
    priorityProperty->setValue(screenPriority);
    backgroundProperty->setValue(screenBackground);
    widthProperty->setValue(screenWidth);
    heightProperty->setValue(screenHeight);

    propertyModel->resetModel();

    QListIterator <Property*> i(propList);

    while (i.hasNext()) {
        propertyModel->addProperty(i.next());
    }
}

void Screen::createPropertyList() {

    idProperty = new TextProperty(trUtf8("Идентификатор"));
    idProperty->setId(SCREEN_ID);
    propList.insert(propList.end(),idProperty);

    titleProperty = new EmptyProperty(trUtf8("Параметры"));
    propList.insert(propList.end(),titleProperty);

    priorityProperty = new EnumProperty(trUtf8("Приоритет"));
    priorityProperty->setId(SCREEN_PRIORITY);
    priorityProperty->setLiterals(QStringList() << trUtf8("Главный") << trUtf8("Вложенный"));
    propList.insert(propList.end(),priorityProperty);

    backgroundProperty = new ColorProperty(trUtf8("Цвет фона"));
    backgroundProperty->setId(SCREEN_BACKGROUND);
    propList.insert(propList.end(),backgroundProperty);

    widthProperty = new IntegerProperty(trUtf8("Ширина"));
    widthProperty->setId(SCREEN_WIDTH);
    widthProperty->setSettings(0,5000);
    propList.insert(propList.end(),widthProperty);

    heightProperty = new IntegerProperty(trUtf8("Высота"));
    heightProperty->setId(SCREEN_HEIGHT);
    heightProperty->setSettings(0,5000);
    propList.insert(propList.end(),heightProperty);
}

void Screen::createContextMenuActions() {

    inGroupAction = new QAction(QIcon(":/images/group.png"),trUtf8("Группировать"),&contextServiceMenu);
    connect(inGroupAction,SIGNAL(triggered()),SLOT(slotGroupElements()));

    outGroupAction = new QAction(QIcon(":/images/ungroup.png"),trUtf8("Разгруппировать"),&contextServiceMenu);

    alignTopAction = new QAction(QIcon(":/images/align-top.png"),trUtf8("Выровнять по верху"),&contextServiceMenu);
    alignTopAction->setData(Qt::AlignTop);
    connect(alignTopAction,SIGNAL(triggered()),SLOT(slotAlignElements()));

    alignDownAction = new QAction(QIcon(":/images/align-bottom.png"),trUtf8("Выровнять по низу"),&contextServiceMenu);
    alignDownAction->setData(Qt::AlignBottom);
    connect(alignDownAction,SIGNAL(triggered()),SLOT(slotAlignElements()));

    alignRightAction = new QAction(QIcon(":/images/align-right.png"),trUtf8("Выровнять по правому краю"),&contextServiceMenu);
    alignRightAction->setData(Qt::AlignRight);
    connect(alignRightAction,SIGNAL(triggered()),SLOT(slotAlignElements()));

    alignLeftAction = new QAction(QIcon(":/images/align-left.png"),trUtf8("Выровнять по левому краю"),&contextServiceMenu);
    alignLeftAction->setData(Qt::AlignLeft);
    connect(alignLeftAction,SIGNAL(triggered()),SLOT(slotAlignElements()));

    frontPlanAction = new QAction(QIcon(),trUtf8("На передний план"),&contextServiceMenu);
    connect(frontPlanAction,SIGNAL(triggered()),SLOT(slotFrontPlanElements()));

    behindPlanAction = new QAction(QIcon(),trUtf8("На задний план"),&contextServiceMenu);
    connect(behindPlanAction,SIGNAL(triggered()),SLOT(slotBehindPlanElements()));

    actionAdditional = new QAction(QIcon(":/images/additional.png"),trUtf8("Дополнительно"),&contextServiceMenu);
    connect(actionAdditional,SIGNAL(triggered()),SLOT(slotShowAdditionalProperties()));

    saveAsLibraryAction = new QAction(QIcon(":/images/library.png"),trUtf8("Сохранить как библиотеку"),&contextMenu);
    connect(saveAsLibraryAction,SIGNAL(triggered()),SLOT(slotSaveAsLibrary()));

    actionRemoveNodePoint = new QAction(QIcon(":/images/delete.png"),trUtf8("Удалить"),&contextNodePointMenu);
    connect(actionRemoveNodePoint,SIGNAL(triggered()),SLOT(slotRemoveNodePoint()));

    actionAddNodePoint = new QAction(QIcon(":/images/add.png"),trUtf8("Добавить"),&contextNodePointMenu);
    connect(actionAddNodePoint,SIGNAL(triggered()),SLOT(slotAddNodePoint()));

    actionDelete = new QAction(QIcon(":/images/delete.png"),trUtf8("Удалить"),&contextMenu);
    actionDelete->setShortcut(QKeySequence::Delete);
    connect(actionDelete,SIGNAL(triggered()),SLOT(slotEditDelete()));

    actionCopy = new QAction(QIcon(":/images/editcopy.png"),trUtf8("Копировать"),&contextMenu);
    actionCopy->setShortcut(QKeySequence::Copy);
    connect(actionCopy,SIGNAL(triggered()),SLOT(slotEditCopy()));

    actionPaste = new QAction(QIcon(":/images/editpaste.png"),trUtf8("Вставить"),&contextMenu);
    actionPaste->setShortcut(QKeySequence::Paste);
    connect(actionPaste,SIGNAL(triggered()),SLOT(slotEditPaste()));

    actionSelectAll = new QAction(QIcon(),trUtf8("Выделить все"),&contextMenu);
    actionSelectAll->setShortcut(QKeySequence::SelectAll);
    connect(actionSelectAll,SIGNAL(triggered()),SLOT(slotSelectAll()));

    contextServiceMenu.setTitle(trUtf8("Инструменты"));
    contextServiceMenu.setIcon(QIcon(":/images/settings.png"));
    contextServiceMenu.addSeparator();
    contextServiceMenu.addAction(alignTopAction);
    contextServiceMenu.addAction(alignDownAction);
    contextServiceMenu.addAction(alignRightAction);
    contextServiceMenu.addAction(alignLeftAction);
    contextServiceMenu.addSeparator();
    contextServiceMenu.addAction(frontPlanAction);
    contextServiceMenu.addAction(behindPlanAction);
    contextServiceMenu.addSeparator();
    //contextServiceMenu.addAction(actionAdditional);

    contextNodePointMenu.setTitle(trUtf8("Узловые точки"));
    contextNodePointMenu.addAction(actionAddNodePoint);
    contextNodePointMenu.addAction(actionRemoveNodePoint);

    contextMenu.addMenu(&contextServiceMenu);
    contextMenu.addMenu(&contextNodePointMenu);
    contextMenu.addAction(saveAsLibraryAction);
    contextMenu.addAction(actionDelete);
    contextMenu.addAction(actionCopy);
    contextMenu.addAction(actionPaste);
    contextMenu.addAction(actionSelectAll);
}

void Screen::updateActions() {

    actionCopy->setEnabled(selectedItems().count() == 0 ? false : true);
    actionDelete->setEnabled(selectedItems().count() == 0 ? false : true);
    saveAsLibraryAction->setEnabled(selectedItems().count() == 0 ? false : true);
    actionAdditional->setEnabled(selectedItems().count() == 1 ? true : false);

    behindPlanAction->setEnabled(selectedItems().count() == 0 ? false : true);
    frontPlanAction->setEnabled(selectedItems().count() == 0 ? false : true);
    alignTopAction->setEnabled(selectedItems().count() < 2 ? false : true);
    alignDownAction->setEnabled(selectedItems().count() < 2 ? false : true);
    alignRightAction->setEnabled(selectedItems().count() < 2 ? false : true);
    alignLeftAction->setEnabled(selectedItems().count() < 2 ? false : true);

    if (selectedItems().isEmpty()) {
        contextNodePointMenu.setEnabled(false);
        return;
    }

    if (selectedItems().first()->type() == PolygonItemType) {
        contextNodePointMenu.setEnabled(true);
    }
    else {
        contextNodePointMenu.setEnabled(false);
    }
}

void Screen::slotAddNodePoint() {

    Figure *fig = static_cast<Figure*>(selectedItems().first());
    fig->addNodePoint();
}

void Screen::slotRemoveNodePoint() {

    Figure *fig = static_cast<Figure*>(selectedItems().first());
    fig->removeNodePoint();
}

void Screen::slotShowAdditionalProperties() {

    //emit signalShowAddProperties();
}

void Screen::slotElementPropertyChanged(Property *property) {

    if (selectedItems().isEmpty()) {
        return;
    }

    currentItem->updateElementProperty(property->getId(),property->getValue());

    unsavedFlag = true;
    emit elementPropertyChanged();

    if (property->getId() == EL_ID) {
        emit elementIdChanged();
    }
}

void Screen::slotSelectionChanged() {

    updateActions();

    if (selectedItems().isEmpty()) {
        propertyModel->resetModel();
        return;
    }

    propertyModel->resetModel();
    currentItem = dynamic_cast<Figure*>(selectedItems().first());

    if (!currentItem) {
        return;
    }

    QListIterator <Property*> i(currentItem->getPropertyList());

    while (i.hasNext()) {
        propertyModel->addProperty(i.next());
    }
}

void Screen::slotElementMoved(QPointF oldPos) {

    Figure *fig = (Figure*)sender();

    m_undoStack->push(new ChangePositionCommand(fig,oldPos));
}

void Screen::slotElementResized(int width, int height,QPointF pos) {

    Figure *fig = (Figure*)sender();
    m_undoStack->push(new ChangeSizeCommand(fig,width,height,pos));
}

void Screen::drawBackground(QPainter *painter, const QRectF &rect) {

    painter->save();

    painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(screenBackground);
    painter->drawRect(rect);

    if (gridVisible) {
        painter->drawPixmap(0,0,gridPixmap);
    }

    painter->restore();
}

QUndoStack *Screen::undoStack() const {
    return m_undoStack;
}

void Screen::setFileName(const QString &file) {
    filename = file;
}

QString Screen::getFileName() const {
    return filename;
}

void Screen::setScreenId(const QString &id) {
    screenId = id;
    emit changeScreenName();
}

QString Screen::getScreenId() const {
    return screenId;
}

void Screen::setScreenPriority(const QString &priority) {
    screenPriority =  priority;
}

QString Screen::getScreenPriority() const {
    return screenPriority;
}

void Screen::setScreenBackground(const QColor &color) {
    screenBackground = color;
    setBackgroundBrush(screenBackground);
}

QColor Screen::getScreenBackground() const {
    return screenBackground;
}

int Screen::getScreenWidth() const {
    return screenWidth;
}

void Screen::setScreenWidth(int width) {
    screenWidth = width;
    setSceneRect(0,0,screenWidth,screenHeight);
}

int Screen::getScreenHeight() const {
    return screenHeight;
}

void Screen::setScreenHeight(int height) {
    screenHeight = height;
    setSceneRect(0,0,screenWidth,screenHeight);
}

void Screen::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {

    if (event->mimeData()->hasText()) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
        event->setAccepted(true);
    }
    else if (event->mimeData()->hasFormat("rti/lib")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
        event->setAccepted(true);
    }
    else {
        event->setAccepted(false);
    }
}

void Screen::dropEvent(QGraphicsSceneDragDropEvent *event) {

    QPointF position = event->scenePos();

    if (event->mimeData()->hasFormat(QString("rti/lib"))) {
        loadLibrary(event->mimeData()->data("rti/lib"));
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }

    if (event->mimeData()->hasText()) {
        createItems(event->mimeData()->text(),position);
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void Screen::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {

    if (event->mimeData()->hasText()) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void Screen::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {

    if (!itemAt(event->scenePos(),QTransform())) {
        fillScreenPropertyModel();
        return;
    }

    return;
}

void Screen::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {

    contextMenu.exec(event->screenPos());
}

void Screen::connectItem(Figure *item) {

    connect(item,SIGNAL(elementMoved(QPointF)),SLOT(slotElementMoved(QPointF)));
    connect(item,SIGNAL(elementResized(int,int,QPointF)),SLOT(slotElementResized(int,int,QPointF)));
}

void Screen::createItems(const QString &typeId, QPointF position) {

    Figure *last = 0;

    if (typeId == trUtf8("Линия")) {
        Figure *line = new ElementLine();
        line->setClickPosition(position);
        last = line;
        connectItem(line);
    }
    else if (typeId == trUtf8("Стрелка")) {
        Figure *arrow = new ElementArrow();
        arrow->setClickPosition(position);
        last = arrow;
        connectItem(arrow);
    }
    else if (typeId == trUtf8("Прямоугольник")) {
        Figure *rect = new ElementRect();
        rect->setClickPosition(position);
        last = rect;
        connectItem(rect);
    }
    else if (typeId == trUtf8("Полигон")) {
        Figure *polygon = new ElementPolygon();
        polygon->setClickPosition(position);
        last = polygon;
        connectItem(polygon);
    }
    else if (typeId == trUtf8("Эллипс")) {
        Figure *ellipse = new ElementEllipse();
        ellipse->setClickPosition(position);
        last = ellipse;
        connectItem(ellipse);
    }
    else if (typeId == trUtf8("Текст")) {
        Figure *text = new ElementText();
        text->setClickPosition(position);
        last = text;
        connectItem(text);
    }

    if (last) {
        QList <QGraphicsItem*> addList;
        addList.push_back(last);
        m_undoStack->push(new AddCommand(addList,this));
        emit newElementAdded();
    }
}

void Screen::slotGroupElements() {

    /** Trash*/
    if (selectedItems().isEmpty()) {
        return;
    }

    ElementGroup *group = new ElementGroup;

    foreach (QGraphicsItem *item, selectedItems()) {
        group->addToGroup(item);
    }

    group->setClickPosition(QPointF(500,500));
    group->setHandlesChildEvents(false);
    addItem(group);
}

void Screen::slotUngroupElements() {

}

void Screen::slotAlignElements() {

    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
        return;

    Qt::Alignment alignment = static_cast<Qt::Alignment>(
            action->data().toInt());

    QList<QGraphicsItem*> items = selectedItems();
    QVector<double> coordinates;
    populateCoordinates(alignment, &coordinates, items);
    double offset;

    if (alignment == Qt::AlignLeft || alignment == Qt::AlignTop) {
        offset = min(coordinates);
    }
    else
    {
        offset = max(coordinates);
    }

    if (alignment == Qt::AlignLeft || alignment == Qt::AlignRight) {
        for (int i = 0; i < items.count(); ++i)
            ((Figure*)items.at(i))->moveTo(offset - coordinates.at(i), 0);
    }
    else {
        for (int i = 0; i < items.count(); ++i)
            ((Figure*)items.at(i))->moveTo(0,offset - coordinates.at(i));
    }
}

void Screen::populateCoordinates(const Qt::Alignment &alignment,
        QVector<double> *coordinates,
        const QList<QGraphicsItem*> &items)
{
    QListIterator<QGraphicsItem*> i(items);
    while (i.hasNext()) {
        QRectF rect = i.next()->sceneBoundingRect();
        switch (alignment) {
            case Qt::AlignLeft:
                coordinates->append(rect.x()); break;
            case Qt::AlignRight:
                coordinates->append(rect.x() + rect.width()); break;
            case Qt::AlignTop:
                coordinates->append(rect.y()); break;
            case Qt::AlignBottom:
                coordinates->append(rect.y() + rect.height()); break;
        }
    }
}

void Screen::slotFrontPlanElements() {

    foreach (QGraphicsItem *item,selectedItems()) {
        Figure *fig = (Figure*)item;
        fig->setElementZValue(fig->getElementZValue() + 1);
    }
}

void Screen::slotBehindPlanElements() {

    foreach (QGraphicsItem *item,selectedItems()) {
        Figure *fig = (Figure*)item;
        fig->setElementZValue(fig->getElementZValue() - 1);
    }
}

void Screen::slotEditDelete() {

    m_undoStack->push(new RemoveCommand(selectedItems(),this));
    propertyModel->resetModel();
    emit elementsDeleted();
}

void Screen::removeElementEvent() {
    emit elementsDeleted();
}

void Screen::addElementEvent() {
    emit newElementAdded();
}

void Screen::keyPressEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Delete) {

        if (!selectedItems().isEmpty()) {
            slotEditDelete();
        }
    }
    else if (event->matches(QKeySequence::Copy)) {
        slotEditCopy();
    }
    else if (event->matches(QKeySequence::Paste)) {
        slotEditPaste();
    }
    else if (event->matches(QKeySequence::SelectAll)) {
        slotSelectAll();
    }
    else if (event->key() == Qt::Key_Up) {
        moveSelectedElements(0,-10);
    }
    else if (event->key() == Qt::Key_Down) {
        moveSelectedElements(0,10);
    }
    else if (event->key() == Qt::Key_Left) {
        moveSelectedElements(-10,0);
    }
    else if (event->key() == Qt::Key_Right) {
        moveSelectedElements(10,0);
    }
}

void Screen::moveSelectedElements(int xOffset, int yOffset) {

    foreach (QGraphicsItem *item,items()) {
        Figure *fig = dynamic_cast<Figure*>(item);

        if (fig) {
            fig->moveTo(xOffset,yOffset);
        }
    }
}

void Screen::slotSelectAll() {

    foreach (QGraphicsItem *item,items()) {
        item->setSelected(true);
    }
}

void Screen::slotEditCopy() {

    QList <QGraphicsItem*> selItems = selectedItems();

    if (selItems.isEmpty()) {
        return;
    }

    copyItems(selItems);
}

void Screen::copyItems(const QList<QGraphicsItem *> &items) {

    QByteArray copiedItems;
    QDataStream out(&copiedItems,QIODevice::WriteOnly);
    writeItems(out,items);
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(MimeType,copiedItems);
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData);
}

void Screen::slotEditPaste() {

    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (!mimeData) {
        return;
    }

    if (mimeData->hasFormat(MimeType)) {
        QByteArray copiedItems = mimeData->data(MimeType);
        QDataStream in(&copiedItems,QIODevice::ReadOnly);
        readItems(in,pasteOffset,true);
    }
}

void Screen::readItems(QDataStream &in, int offset, bool select) {

    int objectType;
    int itemsCount;
    copyList.clear();

    in >> itemsCount;

    for (int i = 0; i < itemsCount; i++) {

        in >> objectType;

        switch (objectType) {
        case LineItemType: {
            ElementLine *line = new ElementLine();
            in >> *line;
            connectItem(line);
            copyList.insert(copyList.end(),line);
            break;
        }
        case ArrowItemType: {
            ElementArrow *arrow = new ElementArrow();
            in >> *arrow;
            connectItem(arrow);
            copyList.insert(copyList.end(),arrow);
            break;
        }
        case EllipseItemType: {
            ElementEllipse *ellipse = new ElementEllipse;
            in >> *ellipse;
            connectItem(ellipse);
            copyList.insert(copyList.end(),ellipse);
            break;
        }
        case RectItemType: {
            ElementRect *rect = new ElementRect();
            in >> *rect;
            connectItem(rect);
            copyList.insert(copyList.end(),rect);
            break;
        }
        case PolygonItemType: {
            ElementPolygon *polygon = new ElementPolygon();
            in >> *polygon;
            connectItem(polygon);
            copyList.insert(copyList.end(),polygon);
            break;
        }
        case TextItemType: {
            ElementText *text = new ElementText();
            in >> *text;
            connectItem(text);
            copyList.insert(copyList.end(),text);
            break;
        }
        }

    } //for

    foreach (QGraphicsItem *item,copyList) {
        Figure *fig = (Figure*)item;
        fig->setSelected(select);
        fig->moveTo(offset,offset);
    }

    m_undoStack->push(new AddCommand(copyList,this));
}

void Screen::writeItems(QDataStream &out, const QList<QGraphicsItem *> &items) {

    out << items.count();

    for (int j = 0; j < items.count(); j++) {

        int type = items[j]->type();
        items[j]->setSelected(false);

        out << type;

        switch (type) {
        case LineItemType: {
            out << *static_cast<ElementLine*>(items[j]);
            break;
        }
        case ArrowItemType: {
            out << *static_cast<ElementArrow*>(items[j]);
            break;
        }
        case EllipseItemType: {
            out << *static_cast<ElementEllipse*>(items[j]);
            break;
        }
        case RectItemType: {
            out << *static_cast<ElementRect*>(items[j]);
            break;
        }
        case PolygonItemType: {
            out << *static_cast<ElementPolygon*>(items[j]);
            break;
        }
        case TextItemType: {
            out << *static_cast<ElementText*>(items[j]);
            break;
        }
        }

    }
}

void Screen::saveAsBinary(const QString &filename) {

    QFile file(filename);
    QFileInfo fi(filename);

    if (screenId != fi.baseName()) {
        QString newName = fi.absolutePath() + "/" + screenId + ".mdscreenbin";
        file.rename(newName);
    }

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0,trUtf8("Ошибка"),trUtf8("Не удается сохранить файл"),
                                 QMessageBox::Ok);
        return;
    }

    QDataStream out(&file);

    out << *this;

    writeItems(out,items());

    m_undoStack->setClean();
    unsavedFlag = false;
    file.close();
    emit screenSaved();
}

void Screen::loadAsBinary(const QString &filename) {

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,trUtf8("Ошибка"),trUtf8("Не удается открыть файл"),
                                 QMessageBox::Ok);
        return;
    }

    QDataStream in(&file);

    in >> *this;

    readItems(in,0,false);

    file.close();
}

void Screen::saveAsXML(const QString &filename) {

    QFile file(filename);
    QFileInfo fi(filename);

    if (screenId != fi.baseName()) {
        QString newName = fi.absolutePath() + "/" + screenId + ".mdscreenxml";
        file.rename(newName);
    }

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0,trUtf8("Ошибка"),trUtf8("Не удается сохранить файл"),
                                 QMessageBox::Ok);
        return;
    }

    XmlScreenConfigWriter writer;
    writer.writeScreenConfig(file,this);

    unsavedFlag = false;
    m_undoStack->setClean();
    file.close();
    emit screenSaved();
}

/**
XML SCREEN LOADING-------------------------------
*/


void Screen::loadAsXML(const QString &filename) {

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,trUtf8("Ошибка"),trUtf8("Не удается открыть файл"),
                                 QMessageBox::Ok);
        return;
    }

    readScreenConfig(file);
    addElementEvent();

    file.close();
}

void Screen::readScreenConfig(QFile &file) {

    QXmlStreamReader reader;
    reader.setDevice(&file);

    while (!reader.atEnd() && !reader.hasError()) {

        QXmlStreamReader::TokenType token = reader.readNext();

        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {

            if (reader.name() == "screen") {
                readScreenTag(reader);
            }
        }
    }
}

void Screen::readScreenTag(QXmlStreamReader &xml) {

    setScreenAttributes(xml);
    copyList.clear();
    xml.readNext();

    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "screen")) {

        if (xml.tokenType() == QXmlStreamReader::StartElement) {

            if (xml.name() == "element") {

                if (xml.attributes().hasAttribute("internalType")) {
                    Figure *fig = createFigure(xml.attributes().value("internalType").toString());
                    if (fig) {
                        fig->readFromXml(xml.attributes());
                        connectItem(fig);
                        addItem(fig);
                    }
                }
            }
        }

        xml.readNext();
    }
}

void Screen::setScreenAttributes(QXmlStreamReader &xml) {

    if (xml.attributes().hasAttribute("filename")) {
        setFileName(xml.attributes().value("filename").toString());
    }

    if (xml.attributes().hasAttribute("screenId")) {
        setScreenId(xml.attributes().value("screenId").toString());
    }

    if (xml.attributes().hasAttribute("priority")) {
        setScreenPriority(xml.attributes().value("priority").toString());
    }

    if (xml.attributes().hasAttribute("width")) {
        setScreenWidth(xml.attributes().value("width").toString().toInt());
    }

    if (xml.attributes().hasAttribute("height")) {
        setScreenHeight(xml.attributes().value("height").toString().toInt());
    }

    if (xml.attributes().hasAttribute("background")) {
        setScreenBackground(
                    QColor(xml.attributes().value("background").toString()));
    }

    fillGridPixmap();
}

Figure *Screen::createFigure(const QString &internalType) {

    if (internalType == "Arrow") {
        return new ElementArrow();
    }

    if (internalType == "Ellipse") {
        return new ElementEllipse();
    }

    if (internalType == "Line") {
        return new ElementLine();
    }

    if (internalType == "Polygon") {
        return new ElementPolygon();
    }

    if (internalType == "Rect") {
        return new ElementRect();
    }

    if (internalType == "Text") {
        return new ElementText();
    }

    return NULL;
}

/**
XML LIBRARY LOADING-------------------------------
*/

void Screen::loadLibrary(QByteArray &data) {

    QDataStream in(&data,QIODevice::ReadOnly);
    QString filename;
    in >> filename;

    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,trUtf8("Ошибка"),trUtf8("Не удается открыть файл"),
                                 QMessageBox::Ok);
        return;
    }

    readLibraryConfig(file);
    addElementEvent();

    file.close();
}

void Screen::readLibraryConfig(QFile &file) {

    QXmlStreamReader reader;
    reader.setDevice(&file);

    while (!reader.atEnd() && !reader.hasError()) {

        QXmlStreamReader::TokenType token = reader.readNext();

        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {

            if (reader.name() == "Library") {
                readLibraryTag(reader);
            }
        }
    }
}

void Screen::readLibraryTag(QXmlStreamReader &xml) {

    copyList.clear();
    xml.readNext();

    while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Library")) {

        if (xml.tokenType() == QXmlStreamReader::StartElement) {

            if (xml.name() == "element") {

                if (xml.attributes().hasAttribute("internalType")) {
                    Figure *fig = createFigure(xml.attributes().value("internalType").toString());
                    if (fig) {
                        fig->readFromXml(xml.attributes());
                        connectItem(fig);
                        addItem(fig);
                    }
                }
            }
        }

        xml.readNext();
    }
}

void Screen::slotSaveAsLibrary() {

    QString filename = QFileDialog::getSaveFileName(0,trUtf8("Save designer library"),
                                                    QString("."),
                                                    trUtf8("Library (*.mdlib)"));

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0,trUtf8("Ошибка"),trUtf8("Не удается сохранить файл"),
                                 QMessageBox::Ok);
        return;
    }

    XmlScreenConfigWriter writer;
    writer.writeLibraryConfig(file,this);

    file.close();
}

QDataStream &operator<<(QDataStream &out,const Screen &screen) {

    out << screen.getFileName() << screen.getScreenId() << screen.getScreenPriority()
        << screen.getScreenBackground() << screen.getScreenHeight()
        << screen.getScreenWidth();

    return out;
}

QDataStream &operator>>(QDataStream &in,Screen &screen) {

    QString filename;
    QString id;
    QString priority;
    QColor backColor;
    int height;
    int width;

    in >> filename >> id >> priority >> backColor >> height >> width;

    screen.setFileName(filename);
    screen.setScreenId(id);
    screen.setScreenPriority(priority);
    screen.setScreenWidth(width);
    screen.setScreenHeight(height);
    screen.setScreenBackground(backColor);
    screen.fillGridPixmap();

    return in;
}
