#include "longpollexecutor.h"
#include <QJsonDocument>
#include <connection/customnetworkmanager.h>
#include "application.h"

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
    int i =0;
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


void LongPollExecutor::start() {
    started=true;
    sendRequest();

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
                //Новое сообщение
                case 4: {
                    QString messageId = update.value(1).toString();
                    bool isRead = (update.value(2).toString().toUInt() % 2) == 0;

                    QString fromId = update.value(3).toString();
                    uint timestamp = update.value(4).toString().toUInt();
                    QString body = update.value(6).toString();
                    emit messageRecieved(messageId, isRead, fromId, timestamp, body);

                    emit messageRecieved(fromId,!isRead);
                    //4,$message_id,$flags,$from_id,$timestamp,$subject,$text,$attachments -- добавление нового сообщения
                    //TODO Передача сигнала с расширенной информацией о сообщении
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


