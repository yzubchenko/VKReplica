#include "customnetworkmanager.h"
#include <QSslConfiguration>
#include <QNetworkRequest>
#include <QSsl>
#include <connection/cookiejar.h>



CustomNetworkAccessManager::CustomNetworkAccessManager (QSsl::SslProtocol protocol, QSslSocket::PeerVerifyMode mode, QObject* parent) : QNetworkAccessManager(parent) {
    this->protocol = protocol;
    this->mode = mode;
    CookieJar *cookieJar = new CookieJar(this);
    this->setCookieJar(cookieJar);
}

QNetworkReply * CustomNetworkAccessManager::createRequest (Operation op, const QNetworkRequest& req, QIODevice* outgoingData ) {
    QSslConfiguration config = req.sslConfiguration();
    config.setPeerVerifyMode(mode);
    config.setProtocol(protocol);
    QNetworkRequest request(req);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    request.setSslConfiguration(config);
    return QNetworkAccessManager::createRequest(op, request, outgoingData);
}
