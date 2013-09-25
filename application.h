#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include "auth.h"
#include "apimethodexecutor.h"

class ContactModel;

class LongPollExecutor;
class MainWindow;

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(QObject *parent = 0);
    void exec();
    ApiMethodExecutor *getApiMethodExecutor() const {return apiMethodExecutor;}
    ~Application();

    LongPollExecutor *getLongPollExecutor() const {return longPollExecutor;}
    ContactModel *getContactModel() const {return contactModel;}
    QString getUserDisplayName() const {return userDisplayName;}
    QString getUserId() const {return userId;}
    Auth* getAuth() const { return auth; }
    QIcon* getOfflineIcon() const {return offlineIcon;}
    QIcon* getOnlineIcon() const {return onlineIcon;}

private:
    Auth *auth;
    ApiMethodExecutor *apiMethodExecutor;
    LongPollExecutor *longPollExecutor;
    ContactModel *contactModel;
    QString userDisplayName;
    QString userId;
    MainWindow *mainWindow;
    QIcon *offlineIcon = new QIcon(":/contacts/resources/offline.png");
    QIcon *onlineIcon = new QIcon(":/contacts/resources/online.png");
    void applyUser();

    volatile unsigned int networkErrorCounter;
    int maxNetworkErrorCount;

signals:

private slots:
    void onAuthStatusChanged(bool isLogin);
    void onNetworkStatus(bool isOk);
};

#endif // APPLICATION_H
