#include "application.h"
#include "mainwindow.h"
#include "auth.h"
#include <QDebug>

#include <QApplication>
#include <QWidget>

#include <QMap>
#include <QString>
#include "apimethodexecutor.h"

#include <QJsonObject>

Application::Application(QObject *parent) :
    QObject(parent)
{
}

void Application::exec() {
    auth = new Auth();
    auth->exec();

    apiMethodExecutor = new ApiMethodExecutor(auth->getToken());

    MainWindow *mainWindow = new MainWindow(this);
    mainWindow->show();
}
