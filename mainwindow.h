#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QUndoView>
#include <QUndoGroup>
#include <QUndoStack>
#include "objectstreeview.h"
#include "projecttreeview.h"
#include "elementlibrarydialog.h"
#include "screen.h"
#include "newcomponentdialog.h"
#include "propertyeditor/propertymodel.h"
#include "propertyeditor/propertytableview.h"
#include "propertyeditor/propertydialog.h"
#include "deviceeditor/deviceeditordialog.h"
#include "indicationeditor/indicationeditordialog.h"
#include "savestrategydialog.h"
#include "previewwindow.h"
#include "screenmanager.h"
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow,public Ui::MainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    bool isGridVisible() const;

protected:
    void closeEvent(QCloseEvent *);

private:
    void initView();
    void createMenus();
    void createActions();
    void createToolbars();
    void createUndoView();
    void addNewScreen();
    void saveImage();
    QString fixedWindowTitle(const QGraphicsView *viewScreen) const;
    int exitResponse();
    QString getFileName();
    void updateScreenViewInfo(const QString &);
    void updateObjectTree();
    void connectScreen(Screen *screen);
    void disconnectScreen(Screen *screen);
    void saveProject(const QString &);
    void loadProject(const QString &);
    void removeScreen(QGraphicsView *view);
    bool isScreenOpen(const QString &filename);
    bool createDocument(Screen *screen,QGraphicsView *view,const QString &filename);

    QAction *actionShowGraphObj;
    QAction *actionShowTreeObj;
    QAction *actionShowPropEditor;
    QAction *actionShowProjTree;
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSaveProject;
    QAction *actionSaveScreenAs;
    QAction *actionSaveScreen;
    QAction *actionExit;
    QAction *actionShowGrid;
    QAction *actionShowLinear;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionPreview;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCloseScreen;
    QAction *actionCloseAll;
    QAction *actionShowDevDialog;
    QAction *actionShowIndicationDialog;

    Screen *currentScreen;
    QGraphicsView *currentView;

    QTabWidget *screenTabWidget;
    ProjectTreeView *projTree;
    ObjectsTreeView *objTree;
    ElementLibraryDialog *elementDialog;
    PropertyTableView *propertyView;
    PropertyModel *propertyModel;
    PropertyDialog *propertyDialog;
    DeviceEditorDialog *deviceEditor;
    QUndoGroup *undoGroup;
    PreviewWindow *preview;

    bool gridVisible;
    int currentScreenIndex;

public slots:
    void slotNewElementAdded();
    void slotElementsDeleted();
    void slotElementIdChanged();
    void slotElementPropertyChanged();
    void slotScreenPropertyChanged();

private slots:
    QGraphicsView *createTabView();
    void slotShowGraphObj(bool);
    void slotShowTreeObj(bool);
    void slotShowPropEditor(bool);
    void slotShowProjTree(bool);
    void slotEditNew();
    void slotEditOpen();
    void slotSaveProject();
    void slotSaveScreenAs();
    void slotSaveScreen();
    void slotExit();
    void slotShowGrid(bool);
    void slotShowLinear(bool);
    void slotZoomIn();
    void slotZoomOut();
    void slotPreview();
    void slotUpdateActions();
    void slotChangeScreen(int);
    void slotChangeScreenName();
    void slotCloseScreen();
    void slotCloseAll();
    void slotShowElemAddProps();
    void slotShowDevDialog();
    void slotShowIndicationDialog();
};

#endif // MAINWINDOW_H
