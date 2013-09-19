#ifndef DIALOGMANAGER_H
#define DIALOGMANAGER_H

#include "dialog.h"
#include "application.h"
#include "contactmodel.h"
#include <QDialog>

namespace Ui {
class DialogManager;
}

class DialogManager : public QDialog
{
    Q_OBJECT

public:
    explicit DialogManager(Application *application, QWidget *parent = 0);
    ~DialogManager();
    void showDialog(Contact* contact);
public slots:
    void closeDialog(int idx);
private slots:
    void onContactOnlineChange(QString userId, bool isOnline);
private:
    Application *application;
    Ui::DialogManager *ui;
    QMap<QString, Dialog*> *dialogMap;
};

#endif // DIALOGMANAGER_H
