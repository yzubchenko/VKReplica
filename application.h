#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include "auth.h"
#include "apimethodexecutor.h"
#include "dialogmanager.h"


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
    DialogManager getUserId() const {return dialogManager;}
    QString getUserDisplayName() const {return userDisplayName;}
    QString getUserId() const {return userId;}


private:
    Auth *auth;
    ApiMethodExecutor *apiMethodExecutor;
    LongPollExecutor *longPollExecutor;
    QString userDisplayName;
    QString userId;
    DialogManager * dialogManager;

    void applyUser();

signals:

public slots:

};

#endif // APPLICATION_H
