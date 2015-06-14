#include "undocommands.h"
#include <QtDebug>
#include <exception>
/**
Команда добавления нового элемента
*/

AddCommand::AddCommand(const QList<QGraphicsItem *> &addItems, Screen *screen, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    items = addItems;
    this->screen = screen;
}

void AddCommand::undo() {

    foreach (QGraphicsItem *item, items) {
        screen->removeItem(item);
    }
    screen->removeElementEvent();
}

void AddCommand::redo() {

    foreach (QGraphicsItem *item, items) {
        screen->addItem(item);
    }
    screen->addElementEvent();
}

/**
Команда удаления элемента
*/

RemoveCommand::RemoveCommand(const QList<QGraphicsItem *> &delItems, Screen *screen, QUndoCommand *parent)
    : QUndoCommand(parent)
{

    items = delItems;
    this->screen  = screen;
}

void RemoveCommand::undo() {

    foreach (QGraphicsItem *item, items) {
        screen->addItem(item);
    }
    screen->addElementEvent();
}

void RemoveCommand::redo() {

    foreach (QGraphicsItem *item, items) {
        screen->removeItem(item);
    }
    screen->removeElementEvent();
}

/**
Команда изменения положения элемента
*/

ChangePositionCommand::ChangePositionCommand(Figure *fig, QPointF oldPos, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    figure = fig;
    this->oldPos = oldPos;
    newPos = fig->pos();
}

void ChangePositionCommand::undo() {

    figure->setPos(oldPos);
    figure->scene()->update();
}

void ChangePositionCommand::redo() {

    figure->setPos(newPos);
    figure->scene()->update();
}

/**
Команда изменения размера элемента
*/

ChangeSizeCommand::ChangeSizeCommand(Figure *fig, int width, int height,QPointF pos, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    figure = fig;
    oldWidth = width;
    oldHeight = height;
    oldPos = pos;

    newWidth = figure->getElementWidth();
    newHeight = figure->getElementHeight();
    newPos = figure->pos();
}

void ChangeSizeCommand::undo() {

    figure->setElementWidth(oldWidth);
    figure->setElementHeight(oldHeight);
    figure->setElementXPos(oldPos.x());
    figure->setElementYPos(oldPos.y());
    figure->updateBoundingElement();
    figure->scene()->update();
}

void ChangeSizeCommand::redo() {

    figure->setElementWidth(newWidth);
    figure->setElementHeight(newHeight);
    figure->setElementXPos(newPos.x());
    figure->setElementYPos(newPos.y());
    figure->scene()->update();
}
