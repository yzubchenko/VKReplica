#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "audioplayer.h"
#include "contactmodel.h"
#include "dialogmanager.h"

#include <QKeyEvent>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QStandardPaths>

class Application;

namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(const Application* application, QWidget* parent = 0);
    ~MainWindow();
    void applyAuthStatus(const bool& isAuthComplete);
    ContactModel& getContactModel() const {return *contactModel;}
    void closeEvent(QCloseEvent *event);
public slots:
    void showDialog(const QModelIndex& modelIndex) const;
    void applyOnlineStatus(QAction* action);
    void applyOnlineStatus(bool isOnline);
    void onMessage() const;
    void switchContactsVisibility();
    void showAudioPlayer() const;
    void switchSound();
    void onPlayerStateChanged(QMediaPlayer::State state) const;
signals:
    void exit();
private:
    bool isAuthComplete;
    Ui::MainWindow* ui;
    const Application* application;
    ContactModel* contactModel;
    DialogManager* dialogManager;
    AudioPlayer* audioPlayer;

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
