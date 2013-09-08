#ifndef AUTH_H
#define AUTH_H

#include <QObject>
#include <QNetworkReply>
#include <QWebView>
#include <QString>
#include <QMap>
#include <QEventLoop>

class Auth : public QObject
{
    Q_OBJECT
private:
    QWebView *webView;
    QString token;
    QString userId;
    QString expiresIn;
    QEventLoop *eventoLoop;

    QMap<QString, QString>* parseReplyFragment(QString authFragment);
public:
    explicit Auth(QObject *parent = 0);
    void exec();

    QString getToken() const { return token; }

    QString getUserId() const { return userId; }

    QString getExpiresIn() const { return expiresIn; }

    QRect * setWebViewGeometry();
    void calculateWebViewGeometry();
signals:

public slots:
    void handleReply(QNetworkReply *reply);
    void showAuthDialog();

};

#endif // AUTH_H
