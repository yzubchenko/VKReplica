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

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(const Application* application, ContactModel* contactModel, QWidget* parent = 0);
    ~MainWindow();
    void applyAuthStatus(const bool& isAuthComplete);
public slots:
    void showDialog(const QModelIndex& modelIndex) const;
    void applyOnlineStatus(QAction* action);
    void applyOnlineStatus(bool isOnline);
    void onMessage() const;
    void switchContactsVisibility();
    void switchSound();
private:
    bool isAuthComplete;
    Ui::MainWindow *ui;
    const Application *application;
    ContactModel *contactModel;
    DialogManager *dialogManager;

    const QIcon loginIcon = QIcon(":/main_window/resources/loginButton.png");
    const QIcon logoutIcon = QIcon(":/main_window/resources/logoutButton.png");
    const QIcon contactsAllVisibleIcon = QIcon(":/main_window/resources/allVisible.png");
    const QIcon contactsOnlineOnlyIcon = QIcon(":/main_window/resources/onlineOnly.png");
    const QIcon soundOnIcon = QIcon(":/main_window/resources/soundOn.png");
    const QIcon soundOffIcon = QIcon(":/main_window/resources/soundOff.png");

    QMediaPlayer* player;
    const QString messageSoundResourcePath = ":/sounds/resources/message.mp3";
    const QString tempMessageSoundPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1String("/message.mp3");
    void setupStatusButton() const;
    bool contactsAllVisible;
    bool isSoundOn;
    volatile bool isOnline;
};

#endif // MAINWINDOW_H
