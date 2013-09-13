#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include "auth.h"
#include "apimethodexecutor.h"
class ContactModel;

class LongPollExecutor;

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


private:
    Auth *auth;
    ApiMethodExecutor *apiMethodExecutor;
    LongPollExecutor *longPollExecutor;
    ContactModel *contactModel;
    QString userDisplayName;
    QString userId;

    void applyUser();

signals:

public slots:

};

#endif // APPLICATION_H
