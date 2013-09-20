#ifndef DIALOG_H
#define DIALOG_H

#include "application.h"

#include <QWidget>

namespace Ui {
class Dialog;
}

class Dialog : public QWidget
{
    Q_OBJECT

public:
    explicit Dialog(Application *application, QString userId, QWidget *parent = 0);
    QString getUserId() const {return userId;}
    void setUserOnline(bool isOnline);
    ~Dialog();

public slots:
    void insertMessage(QString messageId, bool isOutbox, bool isRead, QString userId, uint timestamp, QString body);
    void markInboxRead();
    void markMessageIsRead(QString messageId);
    void sendMessage();

private slots:
    void scrollToBottom(QSize s);
    void checkSendAvailable();
private:
    Ui::Dialog *ui;
    Application *application;
    QString userId;
    QStringList *unreadInList;

    void setupUi();
    void connectSendMessageTriggers();
    void loadHistory(int count);
    QString prepareMessageHtml(QString messageId, QString fromId, uint timestamp, QString body, bool isRead);
    QWebElement findMessageElement(QString messageId);
};

#endif // DIALOG_H
