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


MainWindow::MainWindow(Application *app, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    this->application = app;
    ui->setupUi(this);
//    const QRect screenRect = QApplication::desktop()->rect();
//    QRect *mainWindowRect = new QRect((screenRect.width()-260), (screenRect.height()-400), 260, 400);
//    this->setGeometry(*mainWindowRect); /**Windows 2 screen bug**/

    ContactDelegate* htmlDelegate = new ContactDelegate();
    ui->listView->setItemDelegate(htmlDelegate);
    this->connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(showDialog(QModelIndex)));
    dialogManager = new DialogManager(application,this);
}

void MainWindow::applyContactModel(ContactModel *contactModel) {
    this->contactModel = contactModel;
    ui->listView->setModel(this->contactModel);
}

void MainWindow::showDialog(QModelIndex modelIndex) {
    Contact *contact = contactModel->getByRow(modelIndex.row());
    dialogManager->showDialog(contact);
}

MainWindow::~MainWindow() {
    delete ui;
    application = NULL;
    dialogManager = NULL;
    contactModel = NULL;
}
