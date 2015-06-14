#include "mainwindow.h"
#include "screenlinkdialog.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),currentScreen(0),currentView(0),
    gridVisible(false),currentScreenIndex(0)
{
    setupUi(this);

    undoGroup = new QUndoGroup(this);
    createActions();
    createMenus();
    createToolbars();
    initView();
    slotUpdateActions();

    setWindowState(Qt::WindowMaximized);
    setWindowTitle(trUtf8("Редактор мнемосхем"));
    setWindowIcon(QIcon(":/images/application.png"));

    connect(screenTabWidget,SIGNAL(currentChanged(int)),SLOT(slotChangeScreen(int)));
}

void MainWindow::initView() {

    /** Компоновка внешнего вида*/
    QSplitter *splitter = new QSplitter(Qt::Horizontal,this);
    screenTabWidget = new QTabWidget;
    projTree = new ProjectTreeView;

    splitter->addWidget(projTree);
    splitter->addWidget(screenTabWidget);
    splitter->setHandleWidth(1);
    splitter->setStretchFactor(0,0);
    splitter->setStretchFactor(1,1);

    horizontalLayout->addWidget(splitter);

    /** Окно выбора графического элемента*/
    elementDialog = new ElementLibraryDialog(this);
    elementDialog->move(pos().x(),pos().y() + height());
    elementDialog->show();

    /** Создание окна свойств элемента*/
    propertyModel = new PropertyModel;
    propertyView = new PropertyTableView(propertyModel);
    propertyDialog = new PropertyDialog(trUtf8("Редактор свойств"),this);
    propertyDialog->setPropertyView(propertyView);
    propertyDialog->show();

    /** Окно предварительного просмотра*/
    preview = new PreviewWindow; //!!!!Долго создается

    /** Окно дерева объектов*/
    objTree = new ObjectsTreeView(this);
    objTree->show();

    /** Окно редактора устройств и сигналов*/
    deviceEditor = new DeviceEditorDialog(this);
}

void MainWindow::createActions() {

    actionShowGraphObj = new QAction(trUtf8("Графические элементы"),this);
    actionShowGraphObj->setCheckable(true);
    actionShowGraphObj->setChecked(true);
    connect(actionShowGraphObj,SIGNAL(triggered(bool)),SLOT(slotShowGraphObj(bool)));

    actionShowTreeObj = new QAction(trUtf8("Дерево объектов"),this);
    actionShowTreeObj->setCheckable(true);
    actionShowTreeObj->setChecked(true);
    connect(actionShowTreeObj,SIGNAL(triggered(bool)),SLOT(slotShowTreeObj(bool)));

    actionShowPropEditor = new QAction(trUtf8("Редактор свойств"),this);
    actionShowPropEditor->setCheckable(true);
    actionShowPropEditor->setChecked(true);
    connect(actionShowPropEditor,SIGNAL(triggered(bool)),SLOT(slotShowPropEditor(bool)));

    actionShowProjTree = new QAction(trUtf8("Дерево проекта"),this);
    actionShowProjTree->setCheckable(true);
    actionShowProjTree->setChecked(true);
    connect(actionShowProjTree,SIGNAL(triggered(bool)),SLOT(slotShowProjTree(bool)));

    actionNew = new QAction(QIcon(":/images/filenew.png"),trUtf8("Новый экран"),this);
    actionNew->setShortcut(QString("Ctrl+N"));
    connect(actionNew,SIGNAL(triggered()),SLOT(slotEditNew()));

    actionOpen = new QAction(QIcon(":/images/fileopen.png"),trUtf8("Открыть проект или экран"),this);
    actionOpen->setShortcut(QString("Ctrl+O"));
    connect(actionOpen,SIGNAL(triggered()),SLOT(slotEditOpen()));

    actionSaveProject = new QAction(QIcon(":/images/filesave.png"),trUtf8("Сохранить все в один проект"),this);
    connect(actionSaveProject,SIGNAL(triggered()),SLOT(slotSaveProject()));

    actionSaveScreenAs = new QAction(trUtf8("Сохранить экран как"),this);
    actionSaveScreenAs->setShortcut(QString("Ctrl+Shift+S"));
    connect(actionSaveScreenAs,SIGNAL(triggered()),SLOT(slotSaveScreenAs()));

    actionSaveScreen = new QAction(trUtf8("Сохранить экран"),this);
    actionSaveScreen->setShortcut(QKeySequence::Save);
    connect(actionSaveScreen,SIGNAL(triggered()),SLOT(slotSaveScreen()));

    actionCloseScreen = new QAction(trUtf8("Закрыть экран"),this);
    connect(actionCloseScreen,SIGNAL(triggered()),SLOT(slotCloseScreen()));

    actionCloseAll = new QAction(trUtf8("Закрыть все"),this);
    connect(actionCloseAll,SIGNAL(triggered()),SLOT(slotCloseAll()));

    actionExit = new QAction(trUtf8("Выход"),this);
    actionExit->setShortcut(QKeySequence::Quit);
    connect(actionExit,SIGNAL(triggered()),SLOT(slotExit()));

    actionShowGrid = new QAction(QIcon(":/images/showgrid.png"),trUtf8("Показать сетку"),this);
    actionShowGrid->setCheckable(true);
    actionShowGrid->setChecked(false);
    connect(actionShowGrid,SIGNAL(triggered(bool)),SLOT(slotShowGrid(bool)));

    actionShowLinear = new QAction(QIcon(":/images/ruler.png"),trUtf8("Показать линейку"),this);
    actionShowLinear->setCheckable(true);
    actionShowLinear->setChecked(false);
    connect(actionShowLinear,SIGNAL(triggered(bool)),SLOT(slotShowLinear(bool)));

    actionZoomIn = new QAction(QIcon(":/images/zoom-in.png"),trUtf8("Увеличить"),this);
    connect(actionZoomIn,SIGNAL(triggered()),SLOT(slotZoomIn()));

    actionZoomOut = new QAction(QIcon(":/images/zoom-out.png"),trUtf8("Уменьшить"),this);
    connect(actionZoomOut,SIGNAL(triggered()),SLOT(slotZoomOut()));

    actionPreview = new QAction(QIcon(":/images/preview.png"),trUtf8("Предварительный просмотр"),this);
    connect(actionPreview,SIGNAL(triggered()),SLOT(slotPreview()));

    actionUndo = undoGroup->createUndoAction(this);
    actionUndo->setIcon(QIcon(":/images/undo.png"));
    actionUndo->setText(trUtf8("Отменить"));
    actionUndo->setShortcut(QKeySequence::Undo);

    actionRedo = undoGroup->createRedoAction(this);
    actionRedo->setText(trUtf8("Повторить"));
    actionRedo->setIcon(QIcon(":/images/redo.png"));
    actionRedo->setShortcut(QKeySequence::Redo);

    actionShowDevDialog = new QAction(QIcon(":/images/deviceeditor.png"),trUtf8("Редактор устройств"),this);
    connect(actionShowDevDialog,SIGNAL(triggered()),SLOT(slotShowDevDialog()));

    actionShowIndicationDialog = new QAction(QIcon(":/images/indication.png"),
                                             trUtf8("Редактор правил индикации мнемознаков"),this);
    connect(actionShowIndicationDialog,SIGNAL(triggered()),SLOT(slotShowIndicationDialog()));
}

void MainWindow::createMenus() {

    QMenu *filemenu = new QMenu(trUtf8("Файл"),this);
    filemenu->addAction(actionNew);
    filemenu->addAction(actionOpen);
    filemenu->addAction(actionSaveProject);
    filemenu->addAction(actionSaveScreenAs);
    filemenu->addAction(actionSaveScreen);
    filemenu->addSeparator();
    filemenu->addAction(actionCloseScreen);
    filemenu->addAction(actionCloseAll);
    filemenu->addSeparator();
    filemenu->addAction(actionExit);

    QMenu *windowMenu = new QMenu(trUtf8("Окно"),this);
    windowMenu->addAction(actionShowGraphObj);
    windowMenu->addAction(actionShowTreeObj);
    windowMenu->addAction(actionShowPropEditor);
    windowMenu->addAction(actionShowProjTree);

    QMainWindow::menuBar()->addMenu(filemenu);
    QMainWindow::menuBar()->addMenu(windowMenu);
}

void MainWindow::createToolbars() {

    toolBar->addAction(actionShowGrid);
    //toolBar->addAction(actionShowLinear);
    toolBar->addAction(actionZoomOut);
    toolBar->addAction(actionZoomIn);
    toolBar->addAction(actionPreview);
    toolBar->addSeparator();
    toolBar->addAction(actionUndo);
    toolBar->addAction(actionRedo);
    toolBar->addSeparator();
    toolBar->addAction(actionShowDevDialog);
    toolBar->addAction(actionShowIndicationDialog);
}

void MainWindow::slotShowDevDialog() {

    deviceEditor->exec();
}

void MainWindow::slotShowIndicationDialog() {

    Figure *figure = dynamic_cast<Figure*>(currentScreen->selectedItems().first());
    IndicationEditorDialog indicationEditor(this);
    indicationEditor.setCurrentEditElement(figure);
    indicationEditor.exec();
}

void MainWindow::closeEvent(QCloseEvent *event) {

    bool unsaved = false;

    QListIterator <Screen*> it(ScreenManager::getInstance()->getScreenList());

    while (it.hasNext()) {
        Screen *screen = it.next();
        if (!screen->undoStack()->isClean() || screen->getUnsavedFlag()) {
            unsaved = true;
        }
    }

    if (unsaved) {
        int r = exitResponse();

        if (r == QMessageBox::Yes) {
            slotSaveProject();
            event->accept();
        }
        else if (r == QMessageBox::No) {
            event->accept();
        }
    }

    if (deviceEditor->getUnsavedFlag()) {

        int r = QMessageBox::information(this,trUtf8("Выход из программы"),
                                         trUtf8("Текущая конфигурация устройств не сохранена.Сохранить?"),
                                         QMessageBox::Yes|QMessageBox::No);
        if (r == QMessageBox::Yes) {
            deviceEditor->slotSave();
            event->accept();
        }
        else if (r == QMessageBox::No) {
            event->accept();
        }
    }

    event->accept();
}

void MainWindow::slotEditNew() {

    NewComponentDialog newDialog(this);
    newDialog.exec();

    if (newDialog.result() == QDialog::Accepted) {

        switch (newDialog.getLastChoose()) {

            /*case NEW_PROJECT:
                break;*/
            case NEW_SCREEN:
                addNewScreen();
                break;
        }
    }
}

QString MainWindow::fixedWindowTitle(const QGraphicsView *viewScreen) const {

    QString title = currentScreen->getScreenId();

    if (title.isEmpty()) {
        title = "Untitled";
    }
    else {
        title = QFileInfo(title).fileName();
    }

    QString result;

    for (int i = 0; ;++i) {
        result = title;

        if (i > 0) {
            result += QString::number(i);
        }

        bool unique = true;

        for (int j = 0; j < screenTabWidget->count(); ++j) {
            const QWidget *widget = screenTabWidget->widget(j);

            if (widget == viewScreen) {
                continue;
            }

            if (result == screenTabWidget->tabText(j)) {
                unique = false;
                break;
            }
        }

        if (unique) {
            break;
        }
    }

    return result;
}

bool MainWindow::isScreenOpen(const QString &filename) {

    QListIterator <Screen*> it(ScreenManager::getInstance()->getScreenList());

    while (it.hasNext()) {
        if (filename == it.next()->getFileName()) {
            return true;
        }
    }

    return false;
}

void MainWindow::addNewScreen() {

    QGraphicsView *view = createTabView();

    if (screenTabWidget->indexOf(view) != -1) {
        delete view;
        return;
    }

    Screen *screen = new Screen(QRectF());
    screen->setGridVisible(gridVisible);
    currentScreen = screen;
    view->setScene(screen);
    currentView = view;
    QString title = fixedWindowTitle(view);
    screen->setScreenId(title);
    screen->setPropertyModel(propertyModel);
    screenTabWidget->addTab(currentView,title);
    screenTabWidget->setCurrentWidget(view);
    projTree->addNewElement(screen->getScreenId());
    ScreenManager::getInstance()->addScreen(screen);

    undoGroup->addStack(screen->undoStack());
    undoGroup->setActiveStack(screen->undoStack());

    connectScreen(screen);
}

void MainWindow::connectScreen(Screen *screen) {

    connect(screen->undoStack(),SIGNAL(indexChanged(int)),SLOT(slotUpdateActions()));
    connect(screen->undoStack(),SIGNAL(cleanChanged(bool)),SLOT(slotUpdateActions()));
    connect(screen,SIGNAL(newElementAdded()),SLOT(slotNewElementAdded()));
    connect(screen,SIGNAL(elementsDeleted()),SLOT(slotElementsDeleted()));
    connect(screen,SIGNAL(elementIdChanged()),SLOT(slotElementIdChanged()));
    connect(screen,SIGNAL(changeScreenName()),SLOT(slotChangeScreenName()));
    connect(screen,SIGNAL(signalShowAddProperties()),SLOT(slotShowElemAddProps()));
    connect(screen,SIGNAL(selectionChanged()),SLOT(slotUpdateActions()));
    connect(screen,SIGNAL(elementPropertyChanged()),SLOT(slotUpdateActions()));
    connect(screen,SIGNAL(screenPropertyChanged()),SLOT(slotUpdateActions()));
    connect(screen,SIGNAL(screenSaved()),SLOT(slotUpdateActions()));
}

void MainWindow::disconnectScreen(Screen *screen) {

    disconnect(screen->undoStack(),SIGNAL(indexChanged(int)),this,SLOT(slotUpdateActions()));
    disconnect(screen->undoStack(),SIGNAL(cleanChanged(bool)),this,SLOT(slotUpdateActions()));
    disconnect(screen,SIGNAL(newElementAdded()),this,SLOT(slotNewElementAdded()));
    disconnect(screen,SIGNAL(elementsDeleted()),this,SLOT(slotElementsDeleted()));
    disconnect(screen,SIGNAL(elementIdChanged()),this,SLOT(slotElementIdChanged()));
    disconnect(screen,SIGNAL(changeScreenName()),this,SLOT(slotChangeScreenName()));
    disconnect(screen,SIGNAL(signalShowAddProperties()),this,SLOT(slotShowElemAddProps()));
    disconnect(screen,SIGNAL(selectionChanged()),this,SLOT(slotUpdateActions()));
    disconnect(screen,SIGNAL(elementPropertyChanged()),this,SLOT(slotUpdateActions()));
    disconnect(screen,SIGNAL(screenPropertyChanged()),this,SLOT(slotUpdateActions()));
    disconnect(screen,SIGNAL(screenSaved()),this,SLOT(slotUpdateActions()));
}

QGraphicsView *MainWindow::createTabView() {

    QGraphicsView *view = new QGraphicsView;
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    return view;
}

void MainWindow::slotShowElemAddProps() {

    ScreenLinkDialog screenDialog(this);
    Figure *fig = static_cast<Figure*>(currentScreen->selectedItems().first());
    screenDialog.fillContent();
    screenDialog.updateContent(fig->getElementId(),fig->getScreenLink());
    screenDialog.exec();

    if (screenDialog.result() == QDialog::Accepted) {
        fig->setScreenLink(screenDialog.getObjectScreenLink());
    }
}

void MainWindow::slotUpdateActions() {

    static const QIcon unsaved(":/images/filesave.png");

    for (int i = 0; i < screenTabWidget->count(); i++) {
        QGraphicsView *view = (QGraphicsView*)screenTabWidget->widget(i);

        if (!((Screen*)view->scene())->undoStack()->isClean() ||
            ((Screen*)view->scene())->getUnsavedFlag())
        {
            screenTabWidget->setTabIcon(screenTabWidget->indexOf(view),unsaved);
        }
        else {
            screenTabWidget->setTabIcon(screenTabWidget->indexOf(view),QIcon());
        }
    }

    actionZoomIn->setEnabled(screenTabWidget->count() ? true : false);
    actionZoomOut->setEnabled(screenTabWidget->count() ? true : false);
    actionPreview->setEnabled(screenTabWidget->count() ? true : false);
    actionShowGrid->setEnabled(screenTabWidget->count() ? true : false);
    actionShowIndicationDialog->setEnabled(false);

    if (!currentScreen) {
        return;
    }

    actionShowIndicationDialog->setEnabled(currentScreen->selectedItems().count() ? true : false);
    undoGroup->setActiveStack(currentScreen->undoStack());

    if (!currentScreen->undoStack()->isClean() || currentScreen->getUnsavedFlag()) {
        actionSaveScreen->setEnabled(true);
    }
    else {
        actionSaveScreen->setEnabled(false);
    }
}

void MainWindow::slotChangeScreen(int screenNum) {

    if (screenNum == -1) {
        objTree->clearModel();
        propertyModel->resetModel();
        return;
    }

    for (int i = 0; i < screenTabWidget->count(); i++) {
        QGraphicsView *view = (QGraphicsView*)screenTabWidget->widget(i);
        ((Screen*)view->scene())->setActive(false);
    }

    currentView = (QGraphicsView*)screenTabWidget->widget(screenNum);
    currentScreen = (Screen*)currentView->scene();
    currentScreen->setActive(true);
    currentScreen->fillScreenPropertyModel();
    objTree->setContentList(currentScreen->items());
    objTree->updateContent();
    currentScreenIndex = screenNum;
    slotUpdateActions();
}

void MainWindow::slotChangeScreenName() {

    screenTabWidget->setTabText(currentScreenIndex,currentScreen->getScreenId());

    int index = ScreenManager::getInstance()->getIndexByScreen(currentScreen);

    if (index >= 0) {
        projTree->screenChangeName(currentScreenIndex,currentScreen->getScreenId());
    }
}

void MainWindow::updateObjectTree() {

    objTree->setContentList(currentScreen->items());
    objTree->updateContent();
}

void MainWindow::slotElementIdChanged() {

    updateObjectTree();
}

void MainWindow::slotElementPropertyChanged() {

}

void MainWindow::slotScreenPropertyChanged() {

}

void MainWindow::slotNewElementAdded() {

    updateObjectTree();
}

void MainWindow::slotElementsDeleted() {

    updateObjectTree();
}

void MainWindow::slotShowGrid(bool on) {

    QListIterator <Screen*> i(ScreenManager::getInstance()->getScreenList());

    while (i.hasNext()) {
        i.next()->setGridVisible(on);
    }

    gridVisible = on;
}

void MainWindow::slotShowGraphObj(bool on) {

    on ? elementDialog->show() : elementDialog->hide();
}

void MainWindow::slotShowTreeObj(bool on) {

    on ? objTree->show() : objTree->hide();
}

void MainWindow::slotShowPropEditor(bool on) {

    on ? propertyDialog->show() : propertyDialog->hide();
}

void MainWindow::slotShowProjTree(bool on) {

    on ? projTree->show() : projTree->hide();
}

void MainWindow::slotCloseAll() {

    while (screenTabWidget->count()) {
        QGraphicsView *view = static_cast<QGraphicsView*>(screenTabWidget->widget(screenTabWidget->currentIndex()));
        removeScreen(view);
        delete view;
    }

    currentView = 0;
    currentScreen = 0;
    slotUpdateActions();
}

void MainWindow::removeScreen(QGraphicsView *view) {

    int index = screenTabWidget->indexOf(view);
    Screen *screen = static_cast<Screen*>(view->scene());

    if (index == -1)
        return;

    if (!screen->undoStack()->isClean()) {
        int r = exitResponse();

        if (r == QMessageBox::Yes) {
            slotSaveScreen();
        }
    }

    screenTabWidget->removeTab(index);
    undoGroup->removeStack(screen->undoStack());
    ScreenManager::getInstance()->removeScreen(screen);
    projTree->updateView();
    disconnectScreen(screen);
    delete screen;
}

void MainWindow::slotCloseScreen() {

    QGraphicsView *view = currentView;
    removeScreen(view);
    delete view;

    if (screenTabWidget->count() == 0) {
        currentScreen = 0;
        currentView = 0;
    }

    slotUpdateActions();
}

void MainWindow::slotEditOpen() {

    const QString &filename = QFileDialog::getOpenFileName(this,
                                                           trUtf8("Open"),
                                                           ".", trUtf8("Mnemo designer project (*.mdproj);;"
                                                                       "Mnemo designer screen Bin (*.mdscreenbin);;"
                                                                       "Mnemo designer screen XML (*.mdscreenxml)"));
    if (filename.isEmpty()) {
        return;
    }

    if (filename.toLower().endsWith(".mdproj")) {
        loadProject(filename);
    }

    if (filename.toLower().endsWith(".mdscreenbin")) {
        //refactor it!!!

        QGraphicsView *view = createTabView();

        if (screenTabWidget->indexOf(view) != -1) {
            delete view;
            return;
        }

        Screen *screen = new Screen(QRectF());
        if (!createDocument(screen,view,filename)) {
            return;
        }
        screen->loadAsBinary(filename);
    }

    if (filename.toLower().endsWith(".mdscreenxml")) {

        QGraphicsView *view = createTabView();

        if (screenTabWidget->indexOf(view) != -1) {
            delete view;
            return;
        }

        Screen *screen = new Screen(QRectF());
        if (!createDocument(screen,view,filename)) {
            return;
        }
        screen->loadAsXML(filename);
    }
}

bool MainWindow::createDocument(Screen *screen,QGraphicsView *view,const QString &filename) {

    if (isScreenOpen(filename)) {
        QMessageBox::information(this,trUtf8("Ошибка открытия файла"),trUtf8("Файл уже открыт"),QMessageBox::Ok);
        delete screen;
        delete view;
        return false;
    }

    screen->setGridVisible(gridVisible);
    currentScreen = screen;
    view->setScene(screen);
    currentView = view;
    screen->setPropertyModel(propertyModel);
    screenTabWidget->addTab(currentView,screen->getScreenId());
    screenTabWidget->setCurrentWidget(view);
    projTree->addNewElement(screen->getScreenId());
    ScreenManager::getInstance()->addScreen(screen);

    undoGroup->addStack(screen->undoStack());
    undoGroup->setActiveStack(screen->undoStack());

    connectScreen(screen);

    screen->undoStack()->setClean();

    return true;
}

void MainWindow::loadProject(const QString &filename) {

    QFile file(filename);

    QDataStream in(&file);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this,trUtf8("Ошибка"),trUtf8("Не удается открыть файл"),
                                 QMessageBox::Ok);
        return;
    }

    while (!in.atEnd()) {

        QGraphicsView *view = createTabView();

        if (screenTabWidget->indexOf(view) != -1) {
            delete view;
            return;
        }

        Screen *screen = new Screen(QRectF());
        in >> *screen;
        screen->readItems(in,0,false);
        screen->setGridVisible(gridVisible);
        currentScreen = screen;
        view->setScene(screen);
        currentView = view;
        screen->setPropertyModel(propertyModel);
        screenTabWidget->addTab(currentView,screen->getScreenId());
        screenTabWidget->setCurrentWidget(view);
        projTree->addNewElement(screen->getScreenId());
        ScreenManager::getInstance()->addScreen(screen);

        undoGroup->addStack(screen->undoStack());
        undoGroup->setActiveStack(screen->undoStack());

        connectScreen(screen);

        screen->undoStack()->setClean();
    }

    file.close();
}

void MainWindow::slotSaveProject() {

    int r = QMessageBox::information(this,trUtf8("Сохранение"),trUtf8("Сохранить проект?"),
                                     QMessageBox::Ok|QMessageBox::Cancel);

    if (r == QMessageBox::Ok) {
        QString fileName = QFileDialog::getSaveFileName(this,trUtf8("Save as"),
                                                        ".",trUtf8("Mnemo designer (*.mdproj)"));
        if (fileName.isEmpty()) {
            return;
        }

        if (!fileName.toLower().endsWith(".mdproj")) {
            fileName += ".mdproj";
        }

        saveProject(fileName);
    }
    else if (r == QMessageBox::Cancel) {
        return;
    }
}

void MainWindow::saveProject(const QString &filename) {

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this,trUtf8("Ошибка"),trUtf8("Не удается сохранить файл"),
                                 QMessageBox::Ok);
    }

    QDataStream out(&file);

    QListIterator <Screen*> it(ScreenManager::getInstance()->getScreenList());

    while (it.hasNext()) {

        Screen *screen = it.next();
        out << *screen;
        screen->writeItems(out,screen->items());
        screen->undoStack()->setClean();
    }

    file.close();
}

void MainWindow::slotSaveScreenAs() {

    SaveStrategyDialog saveAsDialog(this);
    saveAsDialog.exec();

    if (!currentScreen) {
        return;
    }

    if (saveAsDialog.result() == QDialog::Accepted) {

        switch (saveAsDialog.getLastChoose()) {

            case SAVE_XML: {
                    QString filename = QFileDialog::getSaveFileName(this,trUtf8("Save as"),
                                                                QString("./%1").arg(currentScreen->getScreenId()),
                                                                trUtf8("XML(*.mdscreenxml)"));
                    currentScreen->setFileName(filename);
                    currentScreen->saveAsXML(filename);
                    break;
                }
            case SAVE_BINARY: {
                    QString filename = QFileDialog::getSaveFileName(this,trUtf8("Save as"),
                                                                QString("./%1").arg(currentScreen->getScreenId()),
                                                                trUtf8("Binary(*.mdscreenbin)"));
                    currentScreen->setFileName(filename);
                    currentScreen->saveAsBinary(filename);
                    break;
                }
            case SAVE_IMAGE:
                saveImage();
                break;
        }
    }
}

QString MainWindow::getFileName() {

    QString filename = QFileDialog::getSaveFileName(this,trUtf8("Save as"),
                                                    QString("./%1").arg(currentScreen->getScreenId()),
                                                    trUtf8("Binary(*.mdscreenbin);;XML(*.mdscreenxml)"));
    return filename;
}

void MainWindow::updateScreenViewInfo(const QString &fileName) {

    int index = screenTabWidget->indexOf(currentView);
    QFileInfo fi(fileName);
    currentScreen->setScreenId(fi.baseName());
    screenTabWidget->setTabText(index,fi.baseName());
    slotChangeScreenName();
}

void MainWindow::slotSaveScreen() {

    if (!currentScreen) {
        return;
    }

    for (;;) {
        QString fileName = currentScreen->getFileName();

        if (fileName.isEmpty())
            fileName = getFileName();

        if (fileName.isEmpty())
            break;

        QFileInfo fi(fileName);

        if (fileName.toLower().endsWith(".mdscreenxml")) {
            currentScreen->setFileName(fileName);
            updateScreenViewInfo(fileName);
            currentScreen->saveAsXML(fileName);
        }

        if (fileName.toLower().endsWith(".mdscreenbin")) {
            currentScreen->setFileName(fileName);
            updateScreenViewInfo(fileName);
            currentScreen->saveAsBinary(fileName);
        }

        break;

    }
}

void MainWindow::slotExit() {

    if (screenTabWidget->count() == 0) {
        QApplication::quit();
        return;
    }

    bool unsaved = false;

    QListIterator <Screen*> it(ScreenManager::getInstance()->getScreenList());

    while (it.hasNext()) {
        if (!it.next()->undoStack()->isClean()) {
            unsaved = true;
        }
    }

    if (unsaved) {
        int r = exitResponse();

        if (r == QMessageBox::Yes) {
            slotSaveProject();
            QApplication::quit();
        }
        else if (r == QMessageBox::No) {
            QApplication::quit();
        }
    }

    QApplication::quit();
}

int MainWindow::exitResponse() {

    int r = QMessageBox::information(this,trUtf8("Выход из программы"),
                             trUtf8("Имеются несохраненные изменения. Сохранить?"),
                             QMessageBox::Yes | QMessageBox::No);
    return r;
}

void MainWindow::slotShowLinear(bool on) {
    Q_UNUSED(on)
}

void MainWindow::slotZoomIn() {

    if (currentView) {
        currentView->scale(1.25,1.25);
    }
}

void MainWindow::slotZoomOut() {

    if (currentView) {
        currentView->scale(1/1.25,1/1.25);
    }
}

void MainWindow::slotPreview() {

    if (!currentScreen) {
        return;
    }

    QPixmap pixmap(currentScreen->width(),currentScreen->height());

    {
        QPainter painter(&pixmap);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        currentScreen->render(&painter);
    }

    preview->updatePreview(pixmap);
}

void MainWindow::saveImage() {

    const QString filename = QFileDialog::getSaveFileName(this,trUtf8("Save As"),
                                                          ".",trUtf8("*.jpg"));
    if (filename.isEmpty()) {
        return;
    }

    QImage image(currentScreen->width(),currentScreen->height(),QImage::Format_ARGB32);

    {
        QPainter painter(&image);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

        bool showGrid = currentScreen->isGridVisible();

        if (showGrid) {
            currentScreen->setGridVisible(false);
        }

        QList <QGraphicsItem*> items = currentScreen->selectedItems();
        currentScreen->clearSelection();

        currentScreen->render(&painter);

        if (showGrid) {
            currentScreen->setGridVisible(true);
        }

        foreach (QGraphicsItem *item, items) {
            item->setSelected(true);
        }

        currentScreen->slotSelectionChanged();
    }

    if (!image.save(filename)) {
        QMessageBox::information(this,trUtf8("Ошибка сохранения"),
                                 trUtf8("Не удалось сохранить файл"), QMessageBox::Ok);
    }
}
