#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "contactmodel.h"
#include "dialogmanager.h"

#include <QMainWindow>
#include <QMediaPlayer>
#include <QStandardPaths>


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
    void onMessage();
    void switchContactsVisibility();
private:
    bool isAuthComplete;
    Ui::MainWindow *ui;
    Application *application;
    ContactModel *contactModel;
    DialogManager *dialogManager;

    QIcon* loginIcon;
    QIcon* logoutIcon;
    QIcon* contactsAllVisibleIcon;
    QIcon* contactsOnlineOnlyIcon;

    QMediaPlayer* player;
    QString const messageSoundResourcePath = ":/sounds/resources/message.mp3";
    QString const tempMessageSoundPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1String("/message.mp3");
    void setupStatusButton();
    bool contactsAllVisible;
};

#endif // MAINWINDOW_H
