#include "contactmodel.h"
#include "application.h"
#include <QApplication>
#include <QJsonObject>
#include "longpollexecutor.h"

bool ascDisplayName(const Contact* c1 , const Contact* c2 ) {
    if (c1->hasUnreadMessage && !c2->hasUnreadMessage) {
        return true;
    } else if (!c1->hasUnreadMessage && c2->hasUnreadMessage) {
        return false;
    } //Контакты, от которых есть непрочитанные сообщения в любом случае попадают наверх
    if (c1->isOnline && !c2->isOnline) {
        return true;
    } else if ((c1->isOnline && c2->isOnline) || (!c1->isOnline && !c2->isOnline)) {
        if (c1->displayName < c2->displayName) {
            return true;
        }
    }
    return false;
}

bool descRating(const Contact* c1 , const Contact* c2 ) {
    if (c1->hasUnreadMessage && !c2->hasUnreadMessage) {
        return true;
    } else if (!c1->hasUnreadMessage && c2->hasUnreadMessage) {
        return false;
    } //Контакты, от которых есть непрочитанные сообщения в любом случае попадают наверх
    if (c1->isOnline && !c2->isOnline) {
        return true;
    } else if ((c1->isOnline && c2->isOnline) || (!c1->isOnline && !c2->isOnline)) {
        if (c1->rating > c2->rating) {
            return true;
        }
    }
    return false;

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
    int rating = contactJsonList.size();
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
        Contact *contact = new Contact{userId,rating,displayName,isOnline,false};
        rating--;
        contactList->push_back(contact);
        orderList->push_back(contact->userId);
    }
    switch (sortOrder) {
        case DescRating: {
            qSort(contactList->begin(),contactList->end(),descRating);
            break;
        }
        case AscDisplayName:{
            qSort(contactList->begin(),contactList->end(),ascDisplayName);
            break;
        }
    }
    connect(application->getLongPollExecutor(),SIGNAL(contactIsOnline(QString,bool)),this, SLOT(setContactOnline(QString,bool)));
    connect(application->getLongPollExecutor(),SIGNAL(messageRecieved(QString,bool)),this, SLOT(acceptUnreadMessage(QString,bool)));

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
        QVariant contactVariant;
        contactVariant.setValue(contact);
        return contactVariant;
    }
    return QVariant();
}


bool ContactModel::insertRows(int row, int count, const QModelIndex &parent) {
    if (count < 1 || row < 0 || row > rowCount(parent)) {
        return false;
    }
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int r = 0; r < count; ++r) {
        Contact *emptyContact = new Contact();
        contactList->insert(row, emptyContact);
    }
    endInsertRows();
    return true;
}

bool ContactModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (count <= 0 || row < 0 || (row + count) > rowCount(parent)) {
       return false;
    }
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int r = 0; r < count; ++r) {
        contactList->removeAt(row);
    }
    endRemoveRows();
    return true;
}

QList<Contact*>* ContactModel::getAll() const {
    return contactList;
}

Contact *ContactModel::getByRow(int row) {
    if (row<0 || row>=contactList->size()) {
        return NULL;
    }
    return contactList->at(row);
}

Contact* ContactModel::findByUserId(QString userId) {
    foreach (Contact* contact, *contactList) {
        if (contact->userId == userId) {
            return contact;
        }
    }
    //        TODO Если контакт не найден, попробовать сделать запрос по API,
    //        и только если там не найдется, выходить из метода
    return NULL;
}


void ContactModel::push(Contact* contact)
{
    if (contact->hasUnreadMessage) {
        contactList->push_front(contact); //Контакты, от которых есть непрочитанные сообщения в любом случае попадают наверх
    } else {
        switch(sortOrder) {
            case DescRating: {
                foreach (Contact* sortedContact, *contactList) {
                    if (sortedContact->rating <= contact->rating) {
                        if ((sortedContact->isOnline && !sortedContact->hasUnreadMessage && contact->isOnline) || (!sortedContact->isOnline && !contact->isOnline)) {
                            contactList->insert(contactList->indexOf(sortedContact),contact);
                            break;
                        }
                    }
                }
                break;
            }
            case AscDisplayName: {
                foreach (Contact* sortedContact, *contactList) {
                    if (sortedContact->displayName > contact->displayName) {
                        if ((sortedContact->isOnline && !sortedContact->hasUnreadMessage && contact->isOnline) || (!sortedContact->isOnline && !contact->isOnline)) {
                            contactList->insert(contactList->indexOf(sortedContact),contact);
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
}

void ContactModel::insert(Contact* contact, int idx) {
    if (idx < 0) {
        push(contact);
    } else {
        contactList->insert(idx,contact);
    }
    emit dataChanged(index(0), index(contactList->size()-1));
}

void ContactModel::refreshContact(Contact* contact)
{
    contactList->removeAt(contactList->indexOf(contact));
    this->push(contact);
}

void ContactModel::setContactOnline(QString userId, bool isOnline) {
    Contact* contact = findByUserId(userId);
    if (contact == NULL) {
        return;
    }
    contact->isOnline = isOnline;
    refreshContact(contact);
}

void ContactModel::acceptUnreadMessage(QString userId, bool hasUnread) {
    Contact* contact = findByUserId(userId);
    if (contact == NULL) {
        return;
    }
    contact->hasUnreadMessage = hasUnread;
    refreshContact(contact);
}

