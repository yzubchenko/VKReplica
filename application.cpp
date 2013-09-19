#include "application.h"
#include "contactmodel.h"
#include "mainwindow.h"
#include "auth.h"
#include <QDebug>

#include <QApplication>
#include <QWidget>

#include <QMap>
#include <QString>
#include "apimethodexecutor.h"
#include "longpollexecutor.h"
#include "application.h"
#include "dialog.h"

#include <QJsonObject>

Application::Application(QObject *parent) : QObject(parent) {
}


void Application::exec() {
    auth = new Auth(this);
    auth->exec();

    apiMethodExecutor = new ApiMethodExecutor(auth->getToken(),this);
    longPollExecutor = new LongPollExecutor(this,this);

    applyUser();

    MainWindow *mainWindow = new MainWindow(this);
    mainWindow->show();

    contactModel = new ContactModel(this,this);
    mainWindow->applyContactModel(contactModel);

    longPollExecutor->start();
}

void Application::applyUser() {
    QMap<QString,QString> params;
    params.insert("user_ids",auth->getUserId());

    QJsonObject userJson = apiMethodExecutor->executeMethod("users.get",params);
    QVariantMap user = userJson.value("response").toVariant().toList().value(0).toMap();
    this->userDisplayName = user.value("first_name").toString() + " " + user.value("last_name").toString();
    this->userId = auth->getUserId();
}

Application::~Application() {

}

