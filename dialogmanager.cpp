
#include "dialogmanager.h"
#include "ui_dialogmanager.h"
#include "longpollexecutor.h"

DialogManager::DialogManager(const Application* application, QWidget* parent) : QDialog(parent), ui(new Ui::DialogManager) {
    this->application = application;
    dialogMap = QMap<QString,Dialog*>();
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeDialog(int)));
    connect(&application->getLongPollExecutor(),SIGNAL(contactIsOnline(QString,bool)),this, SLOT(onContactOnlineChange(QString,bool)));
}

DialogManager::~DialogManager() {
    delete ui;
}

void DialogManager::showDialog(const Contact& contact) {
    if (!dialogMap.contains(contact.userId)) {
        Dialog* dialog = new Dialog(application, contact.userId, ui->tabWidget);
        dialog->setUserOnline(contact.isOnline);
        dialogMap.insert(contact.userId,dialog);
        QIcon statusIcon = contact.isOnline ? application->getOnlineIcon() : application->getOfflineIcon();
        this->ui->tabWidget->addTab(dialog, statusIcon, contact.displayName);
    }
    Dialog* dialog = dialogMap.value(contact.userId);
    this->show();
    this->activateWindow();
    this->ui->tabWidget->setCurrentWidget(dialog);
}

void DialogManager::closeDialog(const int& idx) {
    Dialog* dialog = (Dialog*) ui->tabWidget->widget(idx);
    dialogMap.remove(dialog->getUserId());
    ui->tabWidget->removeTab(idx);
    if (ui->tabWidget->count()<1) {
        this->hide();
    }
    dialog->deleteLater();
}

void DialogManager::onContactOnlineChange(const QString& userId, const bool& isOnline) const {
    if (dialogMap.contains(userId)) {
        Dialog* dialog = dialogMap.value(userId);
        dialog->setUserOnline(isOnline);
        int idx = ui->tabWidget->indexOf(dialog);
        QIcon statusIcon = isOnline ? application->getOnlineIcon() : application->getOfflineIcon();
        ui->tabWidget->setTabIcon(idx, statusIcon);
    }
}
