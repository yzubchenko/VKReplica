#include "systemtrayicon.h"
#include "mainwindow.h"
#include "application.h"



SystemTrayIcon::SystemTrayIcon(Application* application, QObject *parent) :
    QSystemTrayIcon(parent),
    application(application)
{
    setToolTip("VK Replica");
    setIcon(this->application->getFavicon());
    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(onActivated(QSystemTrayIcon::ActivationReason)));

    setupContextMenu();
}

SystemTrayIcon::~SystemTrayIcon() {
    contextMenu->deleteLater();
}

void SystemTrayIcon::setupContextMenu() {
    contextMenu = new QMenu();

    exitAction = new QAction(this);
    exitAction->setText("Выход");
    exitAction->setObjectName(QStringLiteral("exitAction"));
    contextMenu->addAction(exitAction);

    connect(contextMenu, SIGNAL(triggered(QAction*)), this, SLOT(onContextActionTriggered(QAction*)));
    setContextMenu(contextMenu);
}

void SystemTrayIcon::onActivated(QSystemTrayIcon::ActivationReason activationReason) {
    switch (activationReason) {
        case Unknown: {
            break;
        }
        case Context: {
            break;
        }
        case DoubleClick: {
            if (application->getMainWindow().isHidden()) {
                application->getMainWindow().show();
            } else {
                application->getMainWindow().hide();
            }
            break;
        }
        case Trigger: {
            break;
        }
        case MiddleClick: {
            break;
        }
        default:  {
            break;
        }
    }
}

void SystemTrayIcon::onContextActionTriggered(QAction* action) {
    if (action == exitAction) {
        application->exit();
    }

}


