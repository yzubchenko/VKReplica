#include "auth.h"
#include <QApplication>
#include <QRect>
#include <QDesktopWidget>
#include <QWidget>

#include "connection/customnetworkmanager.h"

#include <QUrl>
#include <QDebug>
#include <QStandardPaths>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <connection/cookiejar.h>

/*Конструктор*/
Auth::Auth(QWidget *parent) : QWidget(parent) {
    url = new QUrl("https://oauth.vk.com/authorize?client_id=3860301&scope=friends,audio,status,messages&redirect_uri=https://oauth.vk.com/blank.html&display=page&v=5.0&response_type=token");
    webView = new QWebView(parent);
    webView->setWindowTitle("VK Replica - Авторизация");
    Qt::WindowFlags flags = Qt::Window | Qt::WindowCloseButtonHint;
    webView->setWindowFlags(flags);
    webView->setWindowModality(Qt::ApplicationModal);
    calculateWebViewGeometry();
    refreshWebView();
}

/*Расчет геометрии окна авторизации*/
void Auth::calculateWebViewGeometry() {
    //const QRect screenRect = QApplication::desktop()->rect();
    //QRect *webViewRect = new QRect(screenRect.width()/2-304, screenRect.height()/2-157,607,314);
   // webView->setGeometry(*webViewRect); /**Windows 2 screen bug**/
    webView->setFixedSize(QSize(607,314));
}

void Auth::refreshWebView() {
    webView->setHtml("");
    CustomNetworkAccessManager *networkAccessManager = new CustomNetworkAccessManager(QSsl::TlsV1SslV3, QSslSocket::VerifyNone);
    networkAccessManager->connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleReply(QNetworkReply*)));
    webView->page()->setNetworkAccessManager(networkAccessManager);
}

/*Отображает окно логина*/
void Auth::showAuthDialog() {  
    qDebug() << "request auth";

    webView->load(*url);
    webView->show();
}

void Auth::changeAuthStatus() {
    if (webView->isHidden()) {

        if (isLogin) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(
                        this
                        , "Отключение учетной записи", "Вы уверены, что хотите отключить учетную запись и\r\nстереть данные авторизации?"
                        , QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                QString cookiePath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1String("/cookies.ini");
                QFile file(cookiePath);
                file.remove();
                isLogin = false;
                emit authStatusChanged(isLogin);
            }
        } else {
            refreshWebView();
            showAuthDialog();
        }
    } else {
        webView->activateWindow();
    }
}

/*Обработчик ответа на запрос авторизации*/
void Auth::handleReply(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QString url = reply->url().url(QUrl::RemoveFragment);
        if (url == "https://oauth.vk.com/blank.html") {
            QMap<QString, QString>* parsedReply = this->parseReplyFragment(reply->url().fragment());
            webView->hide();
            if (parsedReply->contains("error")) {
                QMessageBox::critical(
                            this
                            , "Ошибка"
                            , parsedReply->take("error_description")
                            , QMessageBox::Ok);
            } else {
                token = parsedReply->take("access_token");
                userId = parsedReply->take("user_id");
                expiresIn = parsedReply->take("expires_in");
                qDebug() << "auth complete\r\n";
                isLogin = true;
                emit authStatusChanged(isLogin);
            }
        }
    } else {
        webView->hide();
        QMessageBox::critical(
                    this
                    , "Ошибка подключения"
                    , "Не удается установить соединение с сервером. Проверьте состояние сети."
                    , QMessageBox::Ok);
        qDebug() << reply->errorString();
    }
    reply->deleteLater();
}

/*Парсит ответ на запрос авторизации*/
QMap<QString, QString>* Auth::parseReplyFragment(QString authFragment) {
    QMap<QString, QString> *replyMap = new QMap<QString, QString>();
    QStringList paramList = authFragment.split("&");
    foreach (QString paramStr, paramList) {
        QStringList parsedParam = paramStr.split("=");
        replyMap->insert(parsedParam[0],parsedParam[1]);
    }
    return replyMap;
}

Auth::~Auth() {
    delete webView;
    token.clear();
    userId.clear();
    expiresIn.clear();
}

