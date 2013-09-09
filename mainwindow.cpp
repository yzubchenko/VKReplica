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
#include "htmldelegate.h"
#include <QList>


MainWindow::MainWindow(Application *app, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    this->application = app;
    ui->setupUi(this);
//    const QRect screenRect = QApplication::desktop()->rect();
//    QRect *mainWindowRect = new QRect((screenRect.width()-260), (screenRect.height()-400), 260, 400);
//    this->setGeometry(*mainWindowRect); /**Windows 2 screen bug**/



    HtmlDelegate* htmlDelegate = new HtmlDelegate();
    ui->listView->setItemDelegate(htmlDelegate);

}

void MainWindow::applyContactModel(ContactModel *contactModel) {
    ui->listView->setModel(contactModel);
    //contactModel->acceptHasUnreadMessage("22389460",true);
}

MainWindow::~MainWindow() {
    delete ui;
    application = NULL;
}
