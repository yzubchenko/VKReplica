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
    Dialog(const Application* application, const QString& userId, QWidget* parent = 0);
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
    void openAttachment(QUrl url);

private:
    Ui::Dialog* ui;
    const Application* application;
    QString userId;
    QStringList unreadInList;

    void setupUi();
    void loadHistory(const int count);
    QString prepareMessageHtml(const QString& messageId, const QString& fromId, const uint& timestamp, const QString& body, const bool& isRead, QList<QString> attachmentHtmls) const;
    QWebElement findMessageElement(const QString& messageId) const;

    void applyReadState(const QString& messageId, const QString& fromId, const bool& isRead);

    const QString ATT_PHOTO = "photo";
    const QString ATT_VIDEO = "video";
    const QString ATT_DOC = "doc";
    const QString ATT_WALL = "wall";
    const QString ATT_WALL_REPLY = "wall_reply";
    QList<QString> prepareAttachmentHtmls(const QVariantList& variantAttachments) const;
private slots:
    void scrollToBottom(const QSize& s) const;
    void checkSendAvailable() const;
};

#endif // DIALOG_H
