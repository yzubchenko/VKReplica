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
    ~Dialog();

public slots:
    void insertMessage(QString messageId, bool isRead, QString fromId, uint timestamp, QString body);
private slots:
    void scrollToBottom(QSize s);
    void markMessageIsRead();
    void scheduleMarkRead();
private:
    Ui::Dialog *ui;
    Application *application;
    QString userId;
    QStringList *unreadInList;

    void setupUi();
    void loadHistory(int count);
    QString prepareMessageHtml(QString messageId, QString fromId, uint timestamp, QString body, bool isRead);

};

#endif // DIALOG_H
