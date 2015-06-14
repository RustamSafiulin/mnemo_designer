#ifndef SCREEN_H
#define SCREEN_H

#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QGraphicsItem>
#include <QMimeData>
#include <QMenu>
#include <QDataStream>
#include <QUndoStack>
#include <QMessageBox>
#include <QClipboard>
#include <QApplication>
#include "graphicselements/elementline.h"
#include "graphicselements/elementarrow.h"
#include "graphicselements/elementrect.h"
#include "graphicselements/elementellipse.h"
#include "graphicselements/elementtext.h"
#include "propertyeditor/propertymodel.h"
#include "propertyeditor/emptyproperty.h"
#include "propertyeditor/textproperty.h"
#include "propertyeditor/colorproperty.h"
#include "propertyeditor/integerproperty.h"
#include "propertyeditor/enumproperty.h"
#include "graphicselements/elementgroup.h"
#include "graphicselements/elementpolygon.h"
#include "undocommands.h"
#include <QFile>

class Screen : public QGraphicsScene
{
    Q_OBJECT

public:
    Screen(const QRectF &rect,QObject *parent = 0);
    void setGridVisible(bool);
    bool isGridVisible() const;

    /** Properties*/
    void setScreenId(const QString &);
    QString getScreenId() const;

    void setScreenPriority(const QString &);
    QString getScreenPriority() const;

    void setScreenBackground(const QColor &);
    QColor getScreenBackground() const;

    void setScreenWidth(int);
    int getScreenWidth() const;

    void setScreenHeight(int);
    int getScreenHeight() const;

    void setPropertyModel(PropertyModel *model);
    QUndoStack *undoStack() const;

    void setActive(bool);
    bool active();

    QString getFileName() const;
    void setFileName(const QString &);

    void fillScreenPropertyModel();

    void saveAsXML(const QString &filename);
    void saveAsBinary(const QString &filename);
    void loadAsXML(const QString &filename);
    void loadAsBinary(const QString &filename);

    void writeItems(QDataStream &out,const QList <QGraphicsItem*> &items);
    void readItems(QDataStream &in,int offset,bool select);

    void addElementEvent();
    void removeElementEvent();
    void cleanPropertyModel();
    void connectItem(Figure *item);
    bool getUnsavedFlag();

    friend QDataStream &operator<<(QDataStream &out,const Screen &screen);
    friend QDataStream &operator>>(QDataStream &in, Screen &screen);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    bool gridVisible;
    Figure *currentItem;
    static const int gridSize = 20;
    static const int pasteOffset = 20;
    QString screenId;
    QString screenPriority;
    QString filename;
    QColor screenBackground;
    int screenWidth;
    int screenHeight;

    bool onActive;
    bool unsavedFlag;

    PropertyModel *propertyModel;
    QUndoStack *m_undoStack;
    QPixmap gridPixmap;
    QList <Property *> propList;
    QList <QGraphicsItem*> copyList;

    TextProperty *idProperty;
    EmptyProperty *titleProperty;
    EnumProperty *priorityProperty;
    ColorProperty *backgroundProperty;
    IntegerProperty *widthProperty;
    IntegerProperty *heightProperty;

    /** Контекстное меню*/
    QMenu contextMenu;
    QMenu contextServiceMenu;
    QMenu contextNodePointMenu;

    QAction *inGroupAction; //Сгруппировать
    QAction *outGroupAction;
    QAction *alignTopAction; //Выравнивание по вертикали
    QAction *alignDownAction; //Выравнивание по низу
    QAction *alignRightAction; //Выравнивание по правой стороне
    QAction *alignLeftAction; //Выравнивание по левой стороне
    QAction *frontPlanAction; //на передний план
    QAction *behindPlanAction; //на задний план
    QAction *saveAsLibraryAction; //Сохранить как библиотеку
    QAction *actionAddNodePoint;
    QAction *actionRemoveNodePoint;
    QAction *actionDelete; //удалить
    QAction *actionCopy; //копировать
    QAction *actionPaste; //вставить
    QAction *actionSelectAll;
    QAction *actionAdditional;

    void createItems(const QString &,QPointF);
    void fillGridPixmap();
    void createPropertyList();
    void createContextMenuActions();
    void populateCoordinates(const Qt::Alignment &,QVector <double> *,
                             const QList <QGraphicsItem*> &);
    void copyItems(const QList <QGraphicsItem*> &items);
    void updateActions();
    void moveSelectedElements(int xOffset,int yOffset);

    //XML LOAD
    void readScreenConfig(QFile &file);
    void readScreenTag(QXmlStreamReader &xml);
    void setScreenAttributes(QXmlStreamReader &xml);
    Figure *createFigure(const QString &internalType);

    void loadLibrary(QByteArray &data);
    void readLibraryConfig(QFile &file);
    void readLibraryTag(QXmlStreamReader &xml);

private slots:
    void slotGroupElements();
    void slotUngroupElements();
    void slotAlignElements();
    void slotFrontPlanElements();
    void slotBehindPlanElements();
    void slotSaveAsLibrary();
    void slotEditDelete();
    void slotEditCopy();
    void slotEditPaste();
    void slotSelectAll();
    void slotShowAdditionalProperties();
    void slotAddNodePoint();
    void slotRemoveNodePoint();

public slots:
    void slotSelectionChanged();
    void slotElementPropertyChanged(Property *);
    void slotScreenPropertyChanged(Property *);
    void slotElementMoved(QPointF);
    void slotElementResized(int,int,QPointF);

signals:
    void newElementAdded();
    void changeScreenName();
    void elementsDeleted();
    void elementIdChanged();
    void signalShowAddProperties();
    void elementPropertyChanged();
    void screenPropertyChanged();
    void screenSaved();
};

#endif // SCREEN_H
