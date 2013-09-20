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
#include "contactdelegate.h"
#include <QList>
#include "longpollexecutor.h"
#include "application.h"

MainWindow::MainWindow(Application *app, ContactModel *contactModel, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    this->application = app;
    this->contactModel = contactModel;
    ui->setupUi(this);
    offlineIcon = new QIcon(":/contacts/resources/offline.png");
    onlineIcon = new QIcon(":/contacts/resources/online.png");
    loginIcon = new QIcon(":/dialog/resources/loginButton.png");
    logoutIcon = new QIcon(":/dialog/resources/logoutButton.png");
//    const QRect screenRect = QApplication::desktop()->rect();
//    QRect *mainWindowRect = new QRect((screenRect.width()-260), (screenRect.height()-400), 260, 400);
//    this->setGeometry(*mainWindowRect); /**Windows 2 screen bug**/
    ui->usernameLabel->setText("Требуется авторизация");
    ContactDelegate* htmlDelegate = new ContactDelegate();
    ui->listView->setItemDelegate(htmlDelegate);
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(showDialog(QModelIndex)));
    connect(ui->authButton, SIGNAL(released()),application->getAuth(), SLOT(changeAuthStatus()));
    setupStatusButton();
}

void MainWindow::applyAuthStatus(bool isAuthComplete) {
    this->isAuthComplete = isAuthComplete;
    if (isAuthComplete) {
        dialogManager = new DialogManager(application,0);
        ui->statusButton->setEnabled(true);
        ui->authButton->setIcon(*logoutIcon);
        applyOnlineStatus(ui->onlineAction);
        ui->usernameLabel->setText(application->getUserDisplayName());
    } else {
        dialogManager->deleteLater();
        ui->statusButton->setEnabled(false);
        ui->authButton->setIcon(*loginIcon);
        applyOnlineStatus(ui->offlineAction);
        ui->usernameLabel->setText("Требуется авторизация");
    }
}

void MainWindow::applyOnlineStatus(QAction* action) {
    if (action == ui->onlineAction) {
        ui->statusButton->setIcon(*onlineIcon);
        ui->statusButton->setText("В сети");

        contactModel->load();
        ui->listView->setModel(contactModel);
        ui->listView->setEnabled(true);
        application->getApiMethodExecutor()->executeMethod("account.setOnline", QMap<QString,QString>());
        application->getLongPollExecutor()->start();
    } else {
        ui->statusButton->setIcon(*offlineIcon);
        ui->statusButton->setText("Не в сети");

        application->getLongPollExecutor()->stop();
        application->getApiMethodExecutor()->executeMethod("account.setOffline", QMap<QString,QString>());
        ui->listView->setEnabled(false);
        ui->listView->setModel(NULL);
        contactModel->unload();

    }
}


void MainWindow::showDialog(QModelIndex modelIndex) {
    Contact *contact = contactModel->getByRow(modelIndex.row());
    dialogManager->showDialog(contact);
}


void MainWindow::setupStatusButton() {

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
