#include "dialogmodel.h"
#include "contactmodel.h"

DialogModel::DialogModel(Application *application, QString userId, QObject *parent) : QAbstractListModel(parent) {
    this->application = application;
    this->userId = userId;
    this->messageList = new QList<Message*>();

    QMap<QString,QString> params;
    params.insert("count", "20");
    params.insert("user_id",this->userId);

    QJsonObject historyJson = application->getApiMethodExecutor()->executeMethod("messages.getHistory",params);
    QVariantList messageList = historyJson.toVariantMap().value("response").toList();
    messageList.removeFirst();
    if (messageList.size()>0) {
        foreach(QVariant messageVariant, messageList) {
            QVariantMap messageMap = messageVariant.toMap();
            //ID сообщения
            QString id = messageMap.value("mid").toString();
            //Имя пользователья, от которого пришло сообщение
            QString fromId = messageMap.value("from_id").toString();
            QString fromDisplayName;
            if (fromId == application->getUserId()) {
                fromDisplayName = application->getUserDisplayName();
            } else {
                fromDisplayName = application->getContactModel()->findByUserId(fromId)->displayName;
            }
            //Отформатированное представление даты
            uint timestamp = messageMap.value("date").toString().toUInt();
            QDateTime date = QDateTime::fromTime_t(timestamp);
            QString formattedDate = date.toString("yyyy-MM-dd hh:mm:ss");
            //Тело сообщения
            QString body = messageMap.value("body").toString();
            //Статус сообщения
            bool isRead = messageMap.value("read_state").toString().toInt() > 0;

            Message *message = new Message{id,fromDisplayName,formattedDate,body,isRead};
            this->messageList->push_front(message);
        }
    }
}

int DialogModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return messageList->count();
}

QVariant DialogModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= messageList->size()) {
        return QVariant();
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        Message *message = messageList->at(index.row());
        QVariant messageVariant;
        messageVariant.setValue(message);
        return messageVariant;
    }
    return QVariant();
}

bool DialogModel::insertRows(int row, int count, const QModelIndex &parent) {
    if (count < 1 || row < 0 || row > rowCount(parent)) {
        return false;
    }
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int r = 0; r < count; ++r) {
        Message *emptyMessage = new Message();
        messageList->insert(row, emptyMessage);
    }
    endInsertRows();
    return true;
}

bool DialogModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (count <= 0 || row < 0 || (row + count) > rowCount(parent)) {
       return false;
    }
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int r = 0; r < count; ++r) {
        messageList->removeAt(row);
    }
    endRemoveRows();
    return true;
}
