#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QRect>
#include <QDesktopWidget>
#include <QStandardItemModel>
#include <QStringList>
#include <QJsonValue>
#include <QVariantList>
#include <QVariantMap>
#include <QVariant>
#include <QMap>


MainWindow::MainWindow(Application *app, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    this->application = app;
    ui->setupUi(this);
    const QRect screenRect = QApplication::desktop()->rect();
    QRect *mainWindowRect = new QRect((screenRect.width()-260), (screenRect.height()-400), 260, 400);
    this->setGeometry(*mainWindowRect);

    QMap<QString, QString> *map = new QMap<QString, QString>();
    map->insert("order", "hints");
    map->insert("fields","online");
    QJsonObject response = this->application->getApiMethodExecutor()->executeMethod("friends.get",*map);



    QVariantList valueList = (response.toVariantMap().take("response")).toList();
    QStandardItemModel *model = new QStandardItemModel(valueList.size(),2,this);
    int i = 0;
    foreach (QVariant value,valueList) {
        QMap<QString,QVariant> valueMap = value.toMap();
        QString displayName =
                valueMap.value("last_name").toString()
                .append(" ")
                .append(valueMap.value("first_name").toString());
        bool isOnline = valueMap.value("online").toDouble();


        model->setItem(i,0,new QStandardItem(isOnline));
        model->setItem(i,1,new QStandardItem(displayName));

        i++;
    }

    ui->tableView->setModel(model);
}

MainWindow::~MainWindow() {
    delete ui;
}
