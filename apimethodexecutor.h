#ifndef APIMETHODEXECUTOR_H
#define APIMETHODEXECUTOR_H

#include <QObject>
#include <QJsonObject>
#include <QMap>
#include <QString>
#include "connection/customnetworkmanager.h"

class ApiMethodExecutor : public QObject {
    Q_OBJECT

public:
    ApiMethodExecutor(QString token, QObject* parent = 0);
    QJsonObject executeMethod(const QString& methodName, const QMap<QString,QString>& params) const;
    ~ApiMethodExecutor();
signals:
    void networkStatus(bool isOk) const;
private:
    CustomNetworkAccessManager* networkAccessManager;
    QString token;
    volatile unsigned int timeoutCounter;
};
#endif // APIMETHODEXECUTOR_H

