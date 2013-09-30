#ifndef DIALOG_H
#define DIALOG_H

#include "application.h"

#include <QWebElement>
#include <QWidget>

namespace Ui {
class Dialog;
}

class Dialog : public QWidget {
    Q_OBJECT

public:
    explicit Dialog(const Application* application, const QString& userId, QWidget* parent = 0);
    const QString& getUserId() const {return userId;}
    void setUserOnline(const bool isOnline) const;
    ~Dialog();


public slots:
    void insertMessage(
            const QString& messageId
            , const bool& isOutbox
            , const bool& isRead
            , const QString& userId
            , const uint& timestamp
            , const QString& body);
    void markInboxRead();
    void markMessageIsRead(const QString& messageId) const;
    void sendMessage() const;

private:
    Ui::Dialog* ui;
    const Application* application;
    QString userId;
    QStringList unreadInList;

    void setupUi();
    void connectSendMessageTriggers() const;
    void loadHistory(const int count);
    QString prepareMessageHtml(const QString& messageId, const QString& fromId, const uint& timestamp, const QString& body, const bool& isRead) const;
    QWebElement findMessageElement(const QString& messageId) const;
    void applyReadState(const QString& messageId, const QString& fromId, const bool& isRead);
private slots:
    void scrollToBottom(const QSize& s) const;
    void checkSendAvailable() const;

};

#endif // DIALOG_H
