#include "dialog.h"
#include "ui_dialog.h"
#include "longpollexecutor.h"
#include <QPaintEngine>
#include "contactmodel.h"
#include <QWebFrame>
#include <QWebElement>
#include <QTimer>



Dialog::Dialog(Application *application, QString userId, QWidget *parent) : QWidget(parent), ui(new Ui::Dialog) {
    this->application = application;
    this->userId = userId;
    unreadInList = new QStringList();
    setupUi();

    connect(ui->textEdit
            , SIGNAL(focusIn())
            , this
            , SLOT(scheduleMarkRead()));
    connect(ui->webView->page()->mainFrame()
            , SIGNAL(contentsSizeChanged(QSize))
            , this
            , SLOT(scrollToBottom(QSize)));

    loadHistory(5);

    connect(application->getLongPollExecutor()
            , SIGNAL(messageRecieved(QString,bool,QString,uint,QString))
            , this
            , SLOT(insertMessage(QString,bool,QString,uint,QString)));
}

void Dialog::setupUi() {
    ui->setupUi(this);
    QList<int> list = QList<int>();
    list << 200 << 100;
    ui->splitter->setSizes(list);
}

void Dialog::loadHistory(int count) {
    QMap<QString,QString> params;
    params.insert("count", QString::number(count));
    params.insert("user_id",userId);

    QJsonObject historyJson = application->getApiMethodExecutor()->executeMethod("messages.getHistory",params);
    QVariantList messageList = historyJson.toVariantMap().value("response").toList();
    messageList.removeFirst();
    QString historyHtml = "";
    if (messageList.size()>0) {
        for(int idx=messageList.size()-1; idx--; idx<=0) {
            QVariantMap messageMap = messageList.value(idx).toMap();

            QString messageId = messageMap.value("mid").toString();
            QString fromId = messageMap.value("from_id").toString();
            uint timestamp = messageMap.value("date").toString().toUInt();
            QString body = messageMap.value("body").toString();
            bool isRead = messageMap.value("read_state").toString().toInt() > 0;

            historyHtml.append(prepareMessageHtml(messageId,fromId,timestamp,body,isRead));
        }
    }
    ui->webView->setHtml(historyHtml);
}

void Dialog::insertMessage(QString messageId, bool isRead, QString fromId, uint timestamp, QString body) {
    QString messageHtml = prepareMessageHtml(messageId, fromId, timestamp, body, isRead);
    ui->webView->setHtml(ui->webView->page()->mainFrame()->toHtml()+messageHtml);
}

QString Dialog::prepareMessageHtml(QString messageId, QString fromId, uint timestamp, QString body, bool isRead) {
    QString displayName;
    if (fromId == application->getUserId()) {
        displayName = application->getUserDisplayName();
    } else {
        displayName = application->getContactModel()->findByUserId(fromId)->displayName;
    }
    QString formattedDate = QDateTime::fromTime_t(timestamp).toString("yyyy-MM-dd hh:mm:ss");

    QString readStateHtml = isRead ? "background-color: #FFFFFF;" : "background-color: #E1E7ED;";
    QString messageHtml =
            "<div id='m"
            + messageId
            + "' style='font-family: tahoma, arial, verdana, sans-serif, Lucida Sans; font-size: 12px; padding-top: 10px;"
            + readStateHtml
            + "'><div style='display:inline; float: left;'><span style='padding-left:5px; padding-top: 5px; color:#2B587A; font-weight: bold;'>"
            + displayName
            + "</span></div><div style='display:inline; float: right;'><span style='padding-right:5px; padding-top: 5px; color:#999;'>"
            + formattedDate
            + "</span></div><br/><div style='padding:5px;'>"
            + body
            + "</div></div>";

    if (!isRead && fromId!=application->getUserId()) {
        unreadInList->push_back(messageId);
    }
    return messageHtml;
}


void Dialog::markMessageIsRead() {
    application->getContactModel()->acceptUnreadMessage(userId,false);
    while (unreadInList->count()>0) {
        QString messageId = unreadInList->takeFirst();
        QString query = "#m"+messageId;
        QWebElement messageElement = ui->webView->page()->mainFrame()->findFirstElement(query);
        if (!messageElement.isNull()) {
            messageElement.setStyleProperty("background-color","#FFFFFF");
        } else {
            qDebug() << "there is no message with id = " + messageId;
        }
    }
}

void Dialog::scheduleMarkRead() {
    QTimer *markReadTimer = new QTimer();
    connect(markReadTimer, SIGNAL(timeout()), this, SLOT(markMessageIsRead()));
    connect(markReadTimer, SIGNAL(timeout()), markReadTimer, SLOT(deleteLater()));
    markReadTimer->start(1000);
}

void Dialog::scrollToBottom(QSize size){
    ui->webView->page()->mainFrame()->setScrollBarValue(Qt::Vertical, size.height());
}

Dialog::~Dialog() {
    delete ui;
}

