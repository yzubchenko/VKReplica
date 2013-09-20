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


private:
    Auth *auth;
    ApiMethodExecutor *apiMethodExecutor;
    LongPollExecutor *longPollExecutor;
    ContactModel *contactModel;
    QString userDisplayName;
    QString userId;
    MainWindow *mainWindow;

    void applyUser();

signals:

private slots:
    void onAuthStatusChanged(bool isLogin);
};

#endif // APPLICATION_H
