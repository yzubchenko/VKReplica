#include "contactmodel.h"
#include <QApplication>
bool lessThen( const Contact* c1 , const Contact* c2 ) {
    if (c1->isOnline && !c2->isOnline) {
        return true;
    } else if ((c1->isOnline && c2->isOnline) || (!c1->isOnline && !c2->isOnline)) {
        if (c1->displayName < c2->displayName) {
            return true;
        }
    } else {
        return false;

    }
}

ContactModel::ContactModel(Application *application, QObject *parent) : QAbstractListModel(parent) {
    this->application = application;

    //Запрос контактов по API
    QMap<QString, QString> *map = new QMap<QString, QString>();
    map->insert("order", "hints");
    map->insert("fields","online");
    QJsonObject response = this->application->getApiMethodExecutor()->executeMethod("friends.get",*map);
    QVariantList contactJsonList = (response.toVariantMap().take("response")).toList();

    //Заполнение списка контактов и списка порядка
    this->contactList = new QList<Contact*>();
    this->orderList = new QList<QString>();
    foreach (QVariant value,contactJsonList) {
        QMap<QString, QVariant> valueMap = value.toMap();
        QString userId = QString::number(
            static_cast< int >(
                (double)valueMap.value("user_id").toDouble()
            )
        );
        QString displayName =
                valueMap.value("first_name").toString()
                .append(" ")
                .append(valueMap.value("last_name").toString());
        bool isOnline = valueMap.value("online").toDouble();
        Contact *contact = new Contact{userId,displayName,isOnline,false};
        contactList->push_back(contact);
        orderList->push_back(contact->userId);
    }
    qSort(contactList->begin(),contactList->end(),lessThen);
}

int ContactModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return contactList->count();
}

QVariant ContactModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= contactList->size()) {
        return QVariant();
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        Contact *contact = contactList->at(index.row());
        QString displayHtml;
        if (contact->unreadMessage) {
            displayHtml = "<img src='"+QApplication::applicationDirPath()+"/resources/msg.png' />&nbsp;&nbsp;";
            if (contact->isOnline) {

                displayHtml += "<b style='color:green; ";
            } else {
                displayHtml += "<span style='color:gray; ";
            }

        } else {
            if (contact->isOnline) {
                displayHtml = "<img src='"+QApplication::applicationDirPath()+"/resources/online.png' />&nbsp;&nbsp;<b style='color:green; ";
            } else {
                displayHtml = "<img src='"+QApplication::applicationDirPath()+"/resources/offline.png' />&nbsp;&nbsp;<span style='color:gray; ";
            }
        }

        displayHtml.append("'>"+contact->displayName+"</b>");
        return displayHtml;
    }
    return QVariant();
}


bool ContactModel::insertRows(int row, int count, const QModelIndex &parent) {
    if (count < 1 || row < 0 || row > rowCount(parent))
            return false;

        beginInsertRows(QModelIndex(), row, row + count - 1);

        for (int r = 0; r < count; ++r) {
            Contact *emptyContact = new Contact();
            contactList->insert(row, emptyContact);
        }

        endInsertRows();

        return true;
}

bool ContactModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (count <= 0 || row < 0 || (row + count) > rowCount(parent))
            return false;

        beginRemoveRows(QModelIndex(), row, row + count - 1);

        for (int r = 0; r < count; ++r)
            contactList->removeAt(row);

        endRemoveRows();

        return true;
}

QList<Contact*>* ContactModel::getAll() const {
    return contactList;
}

void ContactModel::setHasUnreadMessage(QString user_id, bool isRead) {
    if (isRead) {
        contactList->at(0)->unreadMessage = true;
        emit dataChanged(index(0),index(0));
    } else {

    }
}

