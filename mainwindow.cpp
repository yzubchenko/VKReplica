#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "contactmodel.h"
#include <QApplication>
#include <QRect>
#include <QDesktopWidget>
#include <QStringListModel>
#include <QStringList>
#include <QJsonValue>
#include <QVariantList>
#include <QVariantMap>
#include <QFile>
#include "contactdelegate.h"
#include <QList>
#include "longpollexecutor.h"
#include "application.h"

MainWindow::MainWindow(const Application* application, ContactModel* contactModel, QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    this->application = application;
    this->contactModel = contactModel;
    ui->setupUi(this);
    isOnline = false;


//    const QRect screenRect = QApplication::desktop()->rect();
//    QRect *mainWindowRect = new QRect((screenRect.width()-260), (screenRect.height()-400), 260, 400);
//    this->setGeometry(*mainWindowRect); /**Windows 2 screen bug**/
    ui->statusButton->setEnabled(false);
    ui->usernameLabel->setText("Требуется авторизация");
    ContactDelegate* htmlDelegate = new ContactDelegate();
    ui->listView->setItemDelegate(htmlDelegate);
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(showDialog(QModelIndex)));
    connect(ui->authButton, SIGNAL(clicked()),&application->getAuth(), SLOT(changeAuthStatus()));
    contactsAllVisible = true;
    connect(ui->contactsVisibilityButton, SIGNAL(clicked()), this, SLOT(switchContactsVisibility()));
    isSoundOn = true;
    connect(ui->soundButton, SIGNAL(clicked()), this, SLOT(switchSound()));
    setupStatusButton();

    player = new QMediaPlayer(this);
    if (!QFile::exists(tempMessageSoundPath)) {
        QFile::copy(messageSoundResourcePath, tempMessageSoundPath);
        //потому что мы не можем проигрывать звук напрямую из ресурсов
    }
    player->setMedia(QUrl::fromLocalFile(tempMessageSoundPath));
    player->setVolume(100);

}

void MainWindow::applyAuthStatus(const bool& isAuthComplete) {
    this->isAuthComplete = isAuthComplete;
    if (isAuthComplete) {
        dialogManager = new DialogManager(application,0);
        ui->statusButton->setEnabled(true);
        ui->authButton->setIcon(logoutIcon);
        applyOnlineStatus(ui->onlineAction);
        ui->usernameLabel->setText(application->getUserDisplayName());
    } else {
        dialogManager->deleteLater();
        ui->statusButton->setEnabled(false);
        ui->authButton->setIcon(loginIcon);
        applyOnlineStatus(ui->offlineAction);
        ui->usernameLabel->setText("Требуется авторизация");
    }
}

void MainWindow::applyOnlineStatus(QAction* action){
    if (action == ui->onlineAction) {
        if (!isOnline){
            ui->statusButton->setIcon(application->getOnlineIcon());
            ui->statusButton->setText("В сети");

            contactModel->load();
            ui->listView->setModel(contactModel);
            ui->listView->setEnabled(true);
            application->getApiMethodExecutor().executeMethod("account.setOnline", QMap<QString,QString>());
            connect(&application->getLongPollExecutor(),SIGNAL(messageRecieved(QString,bool)), this, SLOT(onMessage()));
            application->getLongPollExecutor().start();
            isOnline = true;
        }
    } else {
        if (isOnline) {
            isOnline = false;
            ui->statusButton->setIcon(application->getOfflineIcon());
            ui->statusButton->setText("Не в сети");

            application->getLongPollExecutor().stop();
            disconnect(&application->getLongPollExecutor(),SIGNAL(messageRecieved(QString,bool)), this, SLOT(onMessage()));
            //application->getApiMethodExecutor()->executeMethod("account.setOffline", QMap<QString,QString>());
            ui->listView->setEnabled(false);
            ui->listView->setModel(NULL);
            contactModel->unload();
        }

    }
}

void MainWindow::applyOnlineStatus(bool isOnline) {
    applyOnlineStatus(isOnline ? ui->onlineAction : ui->offlineAction);
}

void MainWindow::onMessage() const {
    if (isSoundOn) {
        player->setPosition(0);
        player->play();
    }
}

void MainWindow::switchContactsVisibility() {
    contactsAllVisible = !contactsAllVisible;
    ui->contactsVisibilityButton->setIcon(contactsAllVisible ? contactsAllVisibleIcon : contactsOnlineOnlyIcon);
    contactModel->applyContactsVisibility(contactsAllVisible);
}

void MainWindow::switchSound() {
    isSoundOn = !isSoundOn;
    ui->soundButton->setIcon(isSoundOn ? soundOnIcon : soundOffIcon);
}


void MainWindow::showDialog(const QModelIndex& modelIndex) const{
    const Contact* contact = contactModel->getByRow(modelIndex.row());
    if (contact != nullptr) {
        dialogManager->showDialog(*contact);
    }
}


void MainWindow::setupStatusButton() const {
    ui->statusButton->addAction(ui->offlineAction);
    ui->statusButton->addAction(ui->onlineAction);
    connect(ui->statusButton, SIGNAL(triggered(QAction*)), this, SLOT(applyOnlineStatus(QAction*)));
}

MainWindow::~MainWindow() {
    delete ui;
    application = NULL;
    dialogManager = NULL;
    contactModel = NULL;
}
