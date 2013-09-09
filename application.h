#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include "auth.h"
#include "apimethodexecutor.h"

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(QObject *parent = 0);
    void exec();
    ApiMethodExecutor *getApiMethodExecutor() const {return apiMethodExecutor;}
    ~Application();

private:
    Auth *auth;
    ApiMethodExecutor *apiMethodExecutor;
signals:

public slots:

};

#endif // APPLICATION_H
