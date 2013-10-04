#ifndef LONGPOLLEXECUTOR_H
#define LONGPOLLEXECUTOR_H

#include <QNetworkAccessManager>
#include <QObject>

class Application;

class LongPollExecutor : public QObject {
    Q_OBJECT
public:
    LongPollExecutor(const Application *application, QObject *parent = 0);
    void start();
    void stop();
    const bool& isStarted() const {return started;}

public slots:
    void replyFinished(QNetworkReply* reply);
signals:
    void messageRemoved(QString messageId);
    void messageIsRead(QString messageId);
    void messageRecieved(QString fromId, bool isUnread);
    void messageRecieved(QString messageId, bool isOutBox, bool isRead, QString userId, uint timestamp, QString body);
    void contactIsOnline(QString userId, bool isOnline);
    void networkStatus(bool isOk);
private:
    const Application* application;
    QNetworkAccessManager* networkAccessManager;
    QString server;
    QString key;
    QString ts;
    bool started = false;

    const uint F_UNREAD = 1;
    const uint F_OUTBOX = 2;
    const uint F_REPLIED = 4;
    const uint F_IMPORTANT = 8;
    const uint F_CHAT = 16;
    const uint F_FRIENDS = 32;
    const uint F_SPAM = 64;
    const uint F_DELETED = 128;
    const uint F_FIXED = 256;
    const uint F_MEDIA = 512;

    void acquireLongPollServer();
    void sendRequest() const;
    QList<int> parseFlags(int flags) const;
};

#endif // LONGPOLLEXECUTOR_H

/*0,$message_id,0 -- удаление сообщения с указанным local_id
1,$message_id,$flags -- замена флагов сообщения (FLAGS:=$flags)
2,$message_id,$mask[,$user_id] -- установка флагов сообщения (FLAGS|=$mask)
3,$message_id,$mask[,$user_id] -- сброс флагов сообщения (FLAGS&=~$mask)
4,$message_id,$flags,$from_id,$timestamp,$subject,$text,$attachments -- добавление нового сообщения
8,-$user_id,0 -- друг $user_id стал онлайн
9,-$user_id,$flags -- друг $user_id стал оффлайн ($flags равен 0, если пользователь покинул сайт (например, нажал выход) и 1, если оффлайн по таймауту (например, статус away))

51,$chat_id,$self -- один из параметров (состав, тема) беседы $chat_id были изменены. $self - были ли изменения вызываны самим пользователем
61,$user_id,$flags -- пользователь $user_id начал набирать текст в диалоге. событие должно приходить раз в ~5 секунд при постоянном наборе текста. $flags = 1
62,$user_id,$chat_id -- пользователь $user_id начал набирать текст в беседе $chat_id.
70,$user_id,$call_id -- пользователь $user_id совершил звонок имеющий идентификатор $call_id, дополнительную информацию о звонке можно получить используя метод voip.getCallInfo.*/
