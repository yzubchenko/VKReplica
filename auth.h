#ifndef AUTH_H
#define AUTH_H

#include <QObject>
#include <QNetworkReply>
#include <QWebView>
#include <QString>
#include <QMap>
#include <QEventLoop>

class Auth : public QWidget {
    Q_OBJECT

public:
    explicit Auth(QWidget* parent = 0);
    const QString& getToken() const { return token; }
    const QString& getUserId() const { return userId; }
    const QString& getExpiresIn() const { return expiresIn; }
    ~Auth();

public slots:
    void handleReply(QNetworkReply* reply);
    void showAuthDialog() const;
    void changeAuthStatus();

signals:
    void authStatusChanged(bool isLogin);

private:
    QWebView* webView;
    QString token;
    QString userId;
    QString expiresIn;
    QUrl url;
    bool isLogin;

    QMap<QString, QString> parseReplyFragment(const QString authFragment) const;
    void refreshWebView() const;
    void calculateWebViewGeometry() const;
};

#endif // AUTH_H
