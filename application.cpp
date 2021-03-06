#include "application.h"
#include "contactmodel.h"
#include "mainwindow.h"
#include "auth.h"
#include <QDebug>

#include <QApplication>
#include <QMessageBox>
#include <QStandardPaths>
#include <QSystemTrayIcon>
#include <QWidget>

#include <QMap>
#include <QString>
#include "apimethodexecutor.h"
#include "longpollexecutor.h"
#include "application.h"
#include "dialog.h"
#include "mainwindow.h"
#include "systemtrayicon.h"

#include <QJsonObject>

Application::Application(QApplication* qApplication, QObject *parent) :
    QObject(parent),
    qApplication(qApplication),
    networkErrorCounter(0),
    maxNetworkErrorCount(50)
{
    favicon = QIcon(":/main_window/resources/favicon.png");
//    icon.addFile(":/main_window/resources/fav32.png");
//    icon.addFile(":/main_window/resources/fav64.png");
//    icon.addFile(":/main_window/resources/fav128.png");
    //TODO Добавить иконки больших размеров
}


void Application::exec() {
    auth = new Auth();
    connect(auth, SIGNAL(authStatusChanged(bool)), this, SLOT(onAuthStatusChanged(bool)));
    auth->showAuthDialog();

    mainWindow = new MainWindow(this);
    mainWindow->show();

    systemTrayIcon = new SystemTrayIcon(this,this);
    systemTrayIcon->show();
}

void Application::applyUser() {
    QMap<QString,QString> params;
    params.insert("user_ids",auth->getUserId());

    QJsonObject userJson = apiMethodExecutor->executeMethod("users.get",params);
    QVariantMap user = userJson.value("response").toVariant().toList().value(0).toMap();
    this->userDisplayName = user.value("first_name").toString() + " " + user.value("last_name").toString();
    this->userId = auth->getUserId();
}

void Application::onAuthStatusChanged(bool isLogin) {
    if (isLogin) {
        apiMethodExecutor = new ApiMethodExecutor(auth->getToken(),this);
        connect(apiMethodExecutor, SIGNAL(networkStatus(bool)), this, SLOT(onNetworkStatus(bool)));
        longPollExecutor = new LongPollExecutor(this,this);
        connect(longPollExecutor, SIGNAL(networkStatus(bool)), this, SLOT(onNetworkStatus(bool)));
        applyUser();
    } else {
        disconnect(apiMethodExecutor, SIGNAL(networkStatus(bool)), this, SLOT(onNetworkStatus(bool)));
        apiMethodExecutor->deleteLater();
        disconnect(longPollExecutor, SIGNAL(networkStatus(bool)), this, SLOT(onNetworkStatus(bool)));
        longPollExecutor->deleteLater();
        userDisplayName.clear();
        userId.clear();
    }
    mainWindow->applyAuthStatus(isLogin);
}

void Application::onNetworkStatus(bool isOk) {
    if (isOk) {
        networkErrorCounter = 0;
    } else {
        networkErrorCounter++;
        if (networkErrorCounter>=maxNetworkErrorCount) {
            networkErrorCounter = 0;
            mainWindow->applyOnlineStatus(false);
            QMessageBox::critical(
                        mainWindow
                        , "Ошибка подключения"
                        , "Не удается установить соединение с сервером. Проверьте состояние сети."
                        , QMessageBox::Ok);
        }
    }
}

Application::~Application() {
    auth->deleteLater();
}

ContactModel &Application::getContactModel() const {
    return mainWindow->getContactModel();
}

void Application::exit() {
    qDebug() << "Application exit" ;
    systemTrayIcon->hide();
    qApplication->quit();
}

