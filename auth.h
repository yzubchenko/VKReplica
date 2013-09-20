#ifndef AUTH_H
#define AUTH_H

#include <QObject>
#include <QNetworkReply>
#include <QWebView>
#include <QString>
#include <QMap>
#include <QEventLoop>

class Auth : public QWidget
{
    Q_OBJECT
private:
    QWebView *webView;
    QString token;
    QString userId;
    QString expiresIn;
    QUrl *url;
    bool isLogin;

    QMap<QString, QString>* parseReplyFragment(QString authFragment);
    void refreshWebView();
    void calculateWebViewGeometry();
public:
    explicit Auth(QWidget *parent = 0);

    QString getToken() const { return token; }

    QString getUserId() const { return userId; }

    QString getExpiresIn() const { return expiresIn; }


    ~Auth();

signals:
    void authStatusChanged(bool isLogin);
public slots:
    void handleReply(QNetworkReply *reply);
    void showAuthDialog();
    void changeAuthStatus();

};

#endif // AUTH_H
