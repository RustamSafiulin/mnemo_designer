#ifndef SCREENLINKDIALOG_H
#define SCREENLINKDIALOG_H

#include <QDialog>
#include "ui_screenlinkdialog.h"

class ScreenLinkDialog : public QDialog, public Ui::ScreenLinkDialog
{
    Q_OBJECT
    
public:
    explicit ScreenLinkDialog(QWidget *parent = 0);

    QString getObjectScreenLink() const;
    void fillContent();
    void updateContent(const QString &elemId,const QString &screenLink);

private slots:
    void slotApply();
};

#endif // SCREENLINKDIALOG_H
