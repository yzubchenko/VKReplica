#include "dialog.h"
#include "ui_dialog.h"
#include <QPaintEngine>
#include "contactmodel.h"



Dialog::Dialog(Application *application, QString userId, QWidget *parent) : QWidget(parent), ui(new Ui::Dialog) {
    this->application = application;
    this->userId = userId;
    ui->setupUi(this);
    configSplitter();
    
    
    QMap<QString,QString> params;
    params.insert("count", "20");
    params.insert("user_id",userId);

    QJsonObject historyJson = application->getApiMethodExecutor()->executeMethod("messages.getHistory",params);
    QVariantList messageList = historyJson.toVariantMap().value("response").toList();
    messageList.removeFirst();
    if (messageList.size()>0) {
        for(int idx=messageList.size()-1; idx--; idx<=0) {
            QVariantMap messageMap = messageList.value(idx).toMap();
            uint timestamp = messageMap.value("date").toString().toUInt();
            QDateTime date = QDateTime::fromTime_t(timestamp);
            QString formattedDate = date.toString("yyyy-MM-dd hh:mm:ss");

            QString fromId = messageMap.value("from_id").toString();
            QString displayName;
            if (fromId == application->getUserId()) {
                displayName = application->getUserDisplayName();
            } else {
                displayName = application->getContactModel()->findByUserId(fromId)->displayName;
            }
            QString body = messageMap.value("body").toString();

            bool isUnread = messageMap.value("read_state").toString().toInt() <= 0;
            QString readStateWrapper;
            if (isUnread) {
                readStateWrapper = "background-color: #E1E7ED;";
            }
            ui->textBrowser->append("<table width='100%' cellspacing='0' border='0' style='"
                                    + readStateWrapper
                                    + "'><tr><td width='50%' style='padding-left:5px; padding-top: 5px; color:#2B587A; font-weight: bold;'>"
                                    + displayName
                                    + "</td><td width='50%' align=right style='padding-right:5px; padding-top: 5px; color:#999;'>"
                                    + formattedDate
                                    + "</td></tr><tr><td colspan=2 style='padding:5px;'>"
                                    + body
                                    + "</td></tr></table>");
        }
        connect(ui->textEdit, SIGNAL(focusIn()), this, SLOT(onFocusTextEdit()));
    }
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


