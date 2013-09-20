#include "longpollexecutor.h"
#include <QJsonDocument>
#include <connection/customnetworkmanager.h>
#include "application.h"
#include "qmath.h"

LongPollExecutor::LongPollExecutor(Application *application, QObject *parent) : QObject(parent) {
    this->application = application;
    this->networkAccessManager = new CustomNetworkAccessManager(QSsl::TlsV1SslV3, QSslSocket::VerifyNone);
    this->networkAccessManager->connect(
                networkAccessManager
                , SIGNAL(finished(QNetworkReply*))
                , this
                , SLOT(replyFinished(QNetworkReply*)));

    QJsonObject longPollConfigJson = application->getApiMethodExecutor()->executeMethod(
                "messages.getLongPollServer", QMap<QString,QString>());
    QVariantMap longPollConfigMap = longPollConfigJson.toVariantMap().take("response").toMap();
    this->server = longPollConfigMap.value("server").toString();
    this->key = longPollConfigMap.value("key").toString();
    this->ts = longPollConfigMap.value("ts").toString();
}

void LongPollExecutor::sendRequest() {
    QString urlStr = "http://"+server+"?act=a_check&key="+key+"&ts="+ts+"&wait=25&mode=2";
    QUrl *url = new QUrl(urlStr);
    QNetworkRequest *request = new QNetworkRequest(*url);
    request->setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    QSslConfiguration config = request->sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    request->setSslConfiguration(config);
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray buffer;
    networkAccessManager->get(*request);
}

QList<int> LongPollExecutor::parseFlags(int flags) {
    QList<int> parsedFlags;
    int flag = 512;
    while (flags > 0) {
        flag /= 2;
        if (flags >= flag) {
            flags -= flag;
            parsedFlags.push_front(flag);
        }
    }
    return parsedFlags;

}


void LongPollExecutor::start() {
    if (!started) {
        started=true;
        sendRequest();
    }

}

void LongPollExecutor::stop() {
     started=false;
}

void LongPollExecutor::replyFinished(QNetworkReply *reply) {
    if (isStarted()) {
        QString replyJsonStr = QString::fromUtf8(reply->readAll());
        QVariantMap replyMap = QJsonDocument::fromJson(replyJsonStr.toUtf8()).object().toVariantMap();
        ts = replyMap.value("ts").toString();
        QVariantList updates = replyMap.value("updates").toList();
        if (updates.size() > 0) {
            for (int idx=0; idx<updates.size(); idx++) {
                QVariantList update = updates.value(idx).toList();
                int updateType = update.value(0).toInt();
                switch (updateType) {
                //Удаление сообщения
                case 0: {
                    break;
                }
                //Замена флагов сообщения
                case 1: {
                    break;
                }
                //Установка флагов сообщения
                case 2: {
                    break;
                }
                //Cброс флагов сообщения
                case 3: {
                    QList<int> parsedFlags = parseFlags(update.value(2).toString().toUInt());
                    bool isRead = parsedFlags.contains(F_UNREAD);
                    if (isRead) {
                        QString messageId = update.value(1).toString();
                        emit messageIsRead(messageId);
                    }
                    break;
                }
                //Новое сообщение
                case 4: {
                    QList<int> parsedFlags = parseFlags(update.value(2).toString().toUInt());
                    if (!parsedFlags.contains(F_DELETED)) {
                        QString messageId = update.value(1).toString();

                        bool isRead = !parsedFlags.contains(F_UNREAD);
                        bool isOutbox = parsedFlags.contains(F_OUTBOX);
                        QString fromId = update.value(3).toString();
                        uint timestamp = update.value(4).toString().toUInt();
                        QString body = update.value(6).toString();
                        emit messageRecieved(messageId, isOutbox, isRead, fromId, timestamp, body);

                        if (!isOutbox) {
                            emit messageRecieved(fromId,!isRead);
                        }
                    }
                    break;
                }
                //Контакт онлайн
                case 8: {
                    QString userId = update.value(1).toString().remove(0,1);
                    emit contactIsOnline(userId,true);
                    break;
                }
                //Контакт оффлайн
                case 9: {
                    QString userId = update.value(1).toString().remove(0,1);
                    emit contactIsOnline(userId,false);
                    break;
                }
                default: {
                    //TODO: Обработать другие типы обновлений
                    break;
                }

                }
            }
        }
        if (isStarted()) {
            sendRequest();
        }
    }
}


