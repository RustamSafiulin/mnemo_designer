#ifndef ADDSIGNALDIALOG_H
#define ADDSIGNALDIALOG_H

#include <QDialog>
#include "ui_addsignaldialog.h"

class AddSignalDialog : public QDialog, public Ui::AddSignalDialog
{
    Q_OBJECT
    
public:
    explicit AddSignalDialog(QWidget *parent = 0);

    QString getDestination() const;
    QString getName() const;
    QString getDesignation() const;
    QString getCode() const;
    QString getType() const;
    QString getDirection() const;

public slots:
    void slotOk();
    void slotCancel();
};

#endif // ADDSIGNALDIALOG_H
