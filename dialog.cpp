#include "dialog.h"
#include "ui_dialog.h"
#include "longpollexecutor.h"
#include <QPaintEngine>
#include "contactmodel.h"
#include <QWebFrame>
#include <QWebElement>
#include <QTimer>
#include <QShortcut>

Dialog::Dialog(const Application* application, const QString& userId, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Dialog),
    application(application),
    userId(userId)
{
    unreadInList = QStringList();
    setupUi();

    connect(ui->textEdit
            , SIGNAL(focusIn())
            , this
            , SLOT(markInboxRead()));


    connect(ui->webView->page()->mainFrame()
            , SIGNAL(contentsSizeChanged(QSize))
            , this
            , SLOT(scrollToBottom(QSize)));

    loadHistory(20);

    connect(&application->getLongPollExecutor()
            , SIGNAL(messageRecieved(QString,bool, bool,QString,uint,QString))
            , this
            , SLOT(insertMessage(QString,bool, bool,QString,uint,QString)));
    connect(&application->getLongPollExecutor()
            , SIGNAL(messageIsRead(QString))
            , this
            , SLOT(markMessageIsRead(QString)));
    connect(ui->textEdit, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
    connect(ui->pushButton, SIGNAL(released()), this, SLOT(sendMessage()));
}

void Dialog::setupUi() {
    ui->setupUi(this);
    setWindowIcon(this->application->getFavicon());
    QList<int> list = QList<int>();
    list << 200 << 100;
    ui->splitter->setSizes(list);
}

void Dialog::loadHistory(const int count) {
    QMap<QString,QString> params;
    params.insert("count", QString::number(count));
    params.insert("user_id",userId);

    QJsonObject result = application->getApiMethodExecutor().executeMethod("messages.getHistory",params);
    QVariantList messageList = result.toVariantMap().take("response").toMap().take("items").toList();
    //messageList.removeFirst();
    QString historyHtml = "";
    if (messageList.size()>0) {
        for(int idx=messageList.size()-1; idx>=0; idx--) {
            QVariantMap messageMap = messageList.value(idx).toMap();

            QString messageId = messageMap.value("id").toString();
            QString fromId = messageMap.value("from_id").toString();
            uint timestamp = messageMap.value("date").toString().toUInt();
            QString body = messageMap.value("body").toString();
            bool isRead = messageMap.value("read_state").toString().toInt() > 0;

            applyReadState(messageId, fromId, isRead);
            historyHtml.append(prepareMessageHtml(messageId,fromId,timestamp,body,isRead));
        }
    }
    ui->webView->setHtml(historyHtml);
}

void Dialog::applyReadState(const QString& messageId, const QString& fromId, const bool& isRead) {
    if (!isRead && fromId!=application->getUserId()) {
        unreadInList.push_back(messageId);
    }
}

void Dialog::insertMessage(
        const QString& messageId
        , const bool& isOutbox
        , const bool& isRead
        , const QString& userId
        , const uint& timestamp
        , const QString& body) {
    if (userId == this->userId) {
        if (findMessageElement(messageId).isNull()) { //Для защиты от повторного добавления
            QString fromId = isOutbox ? application->getUserId() : userId;
            applyReadState(messageId, fromId, isRead);
            QString messageHtml = prepareMessageHtml(messageId, fromId, timestamp, body, isRead);
            ui->webView->setHtml(ui->webView->page()->mainFrame()->toHtml()+messageHtml);
        }
        if (ui->textEdit->isActiveWindow()) {
            markInboxRead();
        }
    }
}

QString Dialog::prepareMessageHtml(const QString& messageId, const QString& fromId, const uint& timestamp, const QString& body, const bool& isRead) const {
    QString displayName;
    if (fromId == application->getUserId()) {
        displayName = application->getUserDisplayName();
    } else {
        Contact* contact = application->getContactModel().findByUserId(fromId);
        if (contact != nullptr) {
            displayName = contact->displayName;
        } else {
            displayName = "Неизвестный собеседник";
        }
    }
    QString formattedDate = QDateTime::fromTime_t(timestamp).toString("dd.MM.yyyy hh:mm:ss");
    QString readStateHtml = isRead ? "background-color: #FFFFFF;" : "background-color: #E1E7ED;";
    QString messageHtml =
            "<div id='m"
            + messageId
            + "' style='font-family: tahoma, arial, verdana, sans-serif, Lucida Sans; font-size: 11px; padding-top: 10px;"
            + readStateHtml
            + "'><div style='display:inline; float: left;'><span style='padding-left:5px; padding-top: 5px; color:#2B587A; font-weight: bold;'>"
            + displayName
            + "</span></div><div style='display:inline; float: right;'><span style='padding-right:5px; padding-top: 5px; color:#999;'>"
            + formattedDate
            + "</span></div><br/><div style='padding:5px;'>"
            + body
            + "</div></div>";
    return messageHtml;
}

void Dialog::markInboxRead() {
    if (unreadInList.count() > 0) {
        application->getContactModel().acceptUnreadMessage(userId,false);
        QString messageIds = "";
        while (unreadInList.count()>0) {
            QString messageId = unreadInList.takeFirst();
            markMessageIsRead(messageId);
            messageIds.append(messageId);
            if (unreadInList.count()>1) {
                messageIds.append(",");
            }
        }
        QMap<QString,QString> params;
        params.insert("message_ids", messageIds);
        params.insert("user_id",this->userId);
        QJsonObject response = application->getApiMethodExecutor().executeMethod("messages.markAsRead",params);

        if (response.toVariantMap().value("response").toString().toInt() != 1) {
            qDebug() << "mark message as read FAILURE";
        }
    }
}

QWebElement Dialog::findMessageElement(const QString& messageId) const {
    QString query = "#m"+messageId;
    QWebElement messageElement = ui->webView->page()->mainFrame()->findFirstElement(query);
    return messageElement;
}

void Dialog::markMessageIsRead(const QString& messageId) const {
    QWebElement messageElement = findMessageElement(messageId);
    if (!messageElement.isNull()) {
        messageElement.setStyleProperty("background-color","#FFFFFF");
    } else {
        qDebug() << "there is no message with id = " + messageId;
    }

}

void Dialog::sendMessage() const {
    QString messageText = ui->textEdit->toPlainText();
    if (!messageText.isEmpty()) {
        QMap<QString,QString> params;
       // params.insert("user_id", userId);
        params.insert("message", messageText);
        params.insert("user_ids", userId);
        params.insert("type","1");

        QJsonObject response = application->getApiMethodExecutor().executeMethod("messages.send",params);
        ui->textEdit->clear();
    }
}

void Dialog::setUserOnline(const bool isOnline) const{
    QString text = isOnline ? "В сети" : "Не в сети";
    ui->label->setText(text);
}



void Dialog::scrollToBottom(const QSize& size) const{
    ui->webView->page()->mainFrame()->setScrollBarValue(Qt::Vertical, size.height());
}

void Dialog::checkSendAvailable() const {
    bool isEnabled = !ui->textEdit->toPlainText().isEmpty();
    if (ui->pushButton->isEnabled() != isEnabled) {
        ui->pushButton->setEnabled(isEnabled);
    }
}

Dialog::~Dialog() {
    delete ui;
}

