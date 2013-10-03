#ifndef CUSTOMNETWORKACCESSMANAGER_H
#define CUSTOMNETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
#include <QSslConfiguration>
#include <QNetworkRequest>
#include <QSsl>

class CustomNetworkAccessManager : public QNetworkAccessManager {
    Q_OBJECT
private:
    QSsl::SslProtocol protocol;
    QSslSocket::PeerVerifyMode mode;
public:
    CustomNetworkAccessManager (QSsl::SslProtocol protocol, QSslSocket::PeerVerifyMode mode, QObject* parent);

protected:
    QNetworkReply* createRequest(Operation op, const QNetworkRequest& req, QIODevice* outgoingData = 0);
};

#endif // CUSTOMNETWORKACCESSMANAGER_H
