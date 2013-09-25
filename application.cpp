#include "application.h"
#include "contactmodel.h"
#include "mainwindow.h"
#include "auth.h"
#include <QDebug>

#include <QApplication>
#include <QStandardPaths>
#include <QWidget>

#include <QMap>
#include <QString>
#include "apimethodexecutor.h"
#include "longpollexecutor.h"
#include "application.h"
#include "dialog.h"
#include "mainwindow.h"
#include "errordialog.h"

#include <QJsonObject>

Application::Application(QObject *parent) : QObject(parent) {
    networkErrorCounter = 0;
    maxNetworkErrorCount = 50;
}


void Application::exec() {
    auth = new Auth();
    connect(auth, SIGNAL(authStatusChanged(bool)), this, SLOT(onAuthStatusChanged(bool)));
    auth->showAuthDialog();

    contactModel = new ContactModel(this,this);
    mainWindow = new MainWindow(this, contactModel);
    mainWindow->show();


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
        connect(longPollExecutor, SIGNAL(networkStatus()), this, SLOT(onNetworkStatus(bool)));
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
            ErrorDialog *errorDialog = new ErrorDialog("Не удается установить соединение с сервером. Проверьте состояние сети.");
            errorDialog->connect(errorDialog, SIGNAL(finished(int)), errorDialog, SLOT(deleteLater()));
            errorDialog->show();
        }
    }
}

Application::~Application() {

}

