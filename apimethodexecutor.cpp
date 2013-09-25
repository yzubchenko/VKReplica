#include "apimethodexecutor.h"
#include "connection/customnetworkmanager.h"
#include <QUrl>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>

ApiMethodExecutor::ApiMethodExecutor(QString token, QObject *parent) : QObject(parent) {
    this->token = token;
    timeoutCounter = 0;
    networkAccessManager = new CustomNetworkAccessManager(QSsl::TlsV1SslV3, QSslSocket::VerifyNone);
}

QJsonObject ApiMethodExecutor::executeMethod(QString methodName, QMap<QString, QString> params) {
    QString urlStr = "https://api.vk.com/method/" + methodName + "?";
    foreach (QString paramName, params.keys()) {
        urlStr.append(paramName).append("=").append(params.value(paramName)).append("&");
    }
    urlStr.append("v=5.1&");
    urlStr.append("access_token=").append(token);
    qDebug() <<  "API method executing:" << methodName;
    QUrl *url = new QUrl(urlStr);
    QNetworkRequest *request = new QNetworkRequest(*url);
    request->setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    QSslConfiguration config = request->sslConfiguration();
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    request->setSslConfiguration(config);
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray buffer;
    QNetworkReply *reply = networkAccessManager->post(*request,buffer);
    QEventLoop eventLoop;

    networkAccessManager->connect(reply,SIGNAL(finished()),&eventLoop, SLOT(quit()));    
    eventLoop.exec();
    reply->deleteLater();
    if (reply->error() == QNetworkReply::NoError) {
        QString responseJsonStr = QString::fromUtf8(reply->readAll());
        QJsonDocument document = QJsonDocument::fromJson(responseJsonStr.toUtf8());
        emit networkStatus(true);
        return document.object();
    } else {
        qDebug() << "API Method execute: Network Error:" << reply->errorString();
        emit networkStatus(false);
        return executeMethod(methodName,params);
    }
}

ApiMethodExecutor::~ApiMethodExecutor() {
    delete networkAccessManager;
    token.clear();
}
