#include "dialog.h"
#include "ui_dialog.h"
#include <QPaintEngine>
#include "contactmodel.h"
#include "messagedelegate.h"



Dialog::Dialog(Application *application, QString userId, QWidget *parent) : QWidget(parent), ui(new Ui::Dialog) {
    this->application = application;
    this->userId = userId;
    ui->setupUi(this);
    configSplitter();
    MessageDelegate* messageDelegate = new MessageDelegate();
    ui->listView->setItemDelegate(messageDelegate);
    ui->listView->setResizeMode(QListView::Adjust);
    messageDelegate->setParent(ui->listView);

}

void Dialog::applyDialogModel(DialogModel *dialogModel) {
    this->dialogModel = dialogModel;
    ui->listView->setModel(this->dialogModel);
}

void Dialog::configSplitter()
{
    QList<int> list = QList<int>();
    list << 200 << 100;
    ui->splitter->setSizes(list);
}

Dialog::~Dialog() {
    delete ui;
}

void Dialog::onFocusTextEdit() {
    application->getContactModel()->acceptUnreadMessage(userId,false);
}


