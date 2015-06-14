#include "screenmanager.h"

ScreenManager *ScreenManager::instance_ = 0;

ScreenManager::ScreenManager()
{
}

ScreenManager *ScreenManager::getInstance() {

    if (!instance_) {
        instance_ = new ScreenManager;
    }

    return instance_;
}

void ScreenManager::addScreen(Screen *screen) {

    screenList.insert(screenList.end(),screen);
}

void ScreenManager::removeScreen(Screen *screen) {

    screenList.removeAt(screenList.indexOf(screen));
}

Screen *ScreenManager::getScreenById(const QString &id) const {

    QListIterator <Screen*> i(screenList);
    Screen *screen;

    while (i.hasNext()) {

        screen = i.next();

        if (screen->getScreenId().contains(id,Qt::CaseInsensitive)) {
            return screen;
        }
    }

    return 0;
}

int ScreenManager::getIndexByScreen(Screen *screen) {

    return screenList.indexOf(screen);
}

QList <Screen*> ScreenManager::getScreenList() const {
    return screenList;
}
