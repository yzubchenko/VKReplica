
#include "dialogmanager.h"
#include "ui_dialogmanager.h"
#include "longpollexecutor.h"

DialogManager::DialogManager(Application *application, QWidget *parent) : QDialog(parent), ui(new Ui::DialogManager) {
    this->application = application;
    dialogMap = new QMap<QString,Dialog*>();
    ui->setupUi(this);
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeDialog(int)));
    connect(this->application->getLongPollExecutor(),SIGNAL(contactIsOnline(QString,bool)),this, SLOT(onContactOnlineChange(QString,bool)));
}

DialogManager::~DialogManager() {
    delete ui;
}

void DialogManager::showDialog(Contact* contact) {
    if (!dialogMap->contains(contact->userId)) {
        Dialog* dialog= new Dialog(application, contact->userId, this);
        dialog->setUserOnline(contact->isOnline);
        dialogMap->insert(contact->userId,dialog);
        this->ui->tabWidget->addTab(dialog,contact->displayName);
    }
    Dialog *dialog = dialogMap->value(contact->userId);
    this->show();
    this->activateWindow();
    this->ui->tabWidget->setCurrentWidget(dialog);
}

void DialogManager::closeDialog(int idx) {
    Dialog* dialog = (Dialog*) ui->tabWidget->widget(idx);
    dialogMap->remove(dialog->getUserId());
    ui->tabWidget->removeTab(idx);
    if (ui->tabWidget->count()<1) {
        this->hide();
    }
}

void DialogManager::onContactOnlineChange(QString userId, bool isOnline) {
    if (dialogMap->contains(userId)) {
        dialogMap->value(userId)->setUserOnline(isOnline);
    }
}
