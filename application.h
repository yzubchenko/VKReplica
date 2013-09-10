#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include "auth.h"
#include "apimethodexecutor.h"

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

private:
    Auth *auth;
    ApiMethodExecutor *apiMethodExecutor;
    LongPollExecutor *longPollExecutor;
signals:

public slots:

};

#endif // APPLICATION_H
