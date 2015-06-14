#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <QObject>
#include "screen.h"

class ScreenManager : public QObject
{
    Q_OBJECT
public:
    static ScreenManager *getInstance();
    void addScreen(Screen*);
    Screen *getScreenById(const QString &) const;
    int getIndexByScreen(Screen *);
    QList <Screen*> getScreenList() const;
    void removeScreen(Screen *screen);

private:
    ScreenManager();
    static ScreenManager *instance_;
    QList <Screen*> screenList;
};

#endif // SCREENMANAGER_H
