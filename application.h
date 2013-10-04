#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QObject>
#include "auth.h"
#include "apimethodexecutor.h"

class ContactModel;
class LongPollExecutor;
class MainWindow;

class Application : public QObject {
    Q_OBJECT
public:
    explicit Application(QApplication* qApplication, QObject* parent = 0);
    void exec();
    const ApiMethodExecutor& getApiMethodExecutor() const {return *apiMethodExecutor;}
    ~Application();

    LongPollExecutor& getLongPollExecutor() const {return *longPollExecutor;}
    ContactModel& getContactModel() const;
    const QString& getUserDisplayName() const {return userDisplayName;}
    const QString& getUserId() const {return userId;}
    const Auth& getAuth() const { return *auth; }
    const QIcon& getOfflineIcon() const {return offlineIcon;}
    const QIcon& getOnlineIcon() const {return onlineIcon;}

signals:

private:
    QApplication* qApplication;
    Auth* auth;
    ApiMethodExecutor* apiMethodExecutor;
    LongPollExecutor* longPollExecutor;
    QString userDisplayName;
    QString userId;
    MainWindow* mainWindow;
    const QIcon offlineIcon = QIcon(":/contacts/resources/offline.png");
    const QIcon onlineIcon = QIcon(":/contacts/resources/online.png");
    void applyUser();

    volatile unsigned int networkErrorCounter;
    unsigned int maxNetworkErrorCount;

private slots:
    void onAuthStatusChanged(bool isLogin);
    void onNetworkStatus(bool isOk);
};

#endif // APPLICATION_H
