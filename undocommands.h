#ifndef UNDOCOMMANDS_H
#define UNDOCOMMANDS_H

#include <QUndoCommand>
#include "screen.h"
#include "graphicselements/figure.h"

class Screen;

class AddCommand : public QUndoCommand {

public:
    AddCommand(const QList <QGraphicsItem*> &addItems,Screen *screen,QUndoCommand *parent = 0);
    void undo();
    void redo();

private:
    QList <QGraphicsItem*> items;
    Screen *screen;
};

class RemoveCommand : public QUndoCommand {

public:
    RemoveCommand(const QList <QGraphicsItem*> &delItems,Screen *screen,QUndoCommand *parent = 0);
    void undo();
    void redo();

private:
    QList <QGraphicsItem*> items;
    Screen *screen;
};

class ChangePositionCommand : public QUndoCommand {

public:
    ChangePositionCommand(Figure * fig,QPointF oldPos,QUndoCommand *parent = 0);
    void undo();
    void redo();

private:
    Figure *figure;
    QPointF newPos;
    QPointF oldPos;
};

class ChangeSizeCommand : public QUndoCommand {

public:
    ChangeSizeCommand(Figure *fig,int width,int height,QPointF pos,QUndoCommand *parent = 0);
    void undo();
    void redo();

private:
    Figure *figure;
    int oldWidth;
    int oldHeight;
    QPointF oldPos;
    int newWidth;
    int newHeight;
    QPointF newPos;
};

#endif // UNDOCOMMANDS_H
