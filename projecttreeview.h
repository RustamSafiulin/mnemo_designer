#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QListWidget>

class ProjectTreeView : public QListWidget
{
    Q_OBJECT
public:
    ProjectTreeView(QWidget *parent = 0);
    void screenChangeName(int index,const QString &);
    void updateView();

public slots:
    void addNewElement(const QString &);

private:
    QIcon itemIcon;
};

#endif // PROJECTTREEVIEW_H
