#ifndef DIALOGMANAGER_H
#define DIALOGMANAGER_H

#include "dialog.h"
#include "application.h"
#include "contactmodel.h"
#include <QDialog>

namespace Ui {
class DialogManager;
}

class DialogManager : public QDialog {
    Q_OBJECT

public:
    DialogManager(const Application* application, QWidget* parent = 0);
    ~DialogManager();
    void showDialog(const Contact& contact);
public slots:
    void closeDialog(const int& idx);
private:
    const Application* application;
    Ui::DialogManager* ui;
    QMap<QString, Dialog*> dialogMap;
private slots:
    void onContactOnlineChange(const QString& userId, const bool& isOnline) const;
};

#endif // DIALOGMANAGER_H
