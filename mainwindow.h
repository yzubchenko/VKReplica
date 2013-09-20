#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "contactmodel.h"
#include "dialogmanager.h"

#include <QMainWindow>


namespace Ui {
class Application;
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Application *app, ContactModel *contactModel, QWidget *parent = 0);
    ~MainWindow();

    void applyAuthStatus(bool isAuthComplete);
public slots:
    void showDialog(QModelIndex modelIndex);
    void applyOnlineStatus(QAction* action);
private:
    bool isAuthComplete;
    Ui::MainWindow *ui;
    Application *application;
    ContactModel *contactModel;
    DialogManager *dialogManager;

    QIcon* onlineIcon;
    QIcon* offlineIcon;
    QIcon* loginIcon;
    QIcon* logoutIcon;


    void setupStatusButton();
};

#endif // MAINWINDOW_H
