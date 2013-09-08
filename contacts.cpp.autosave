#include "contacts.h"

Contacts::Contacts(Application *application, QObject *parent) : QObject(parent) {
    this->application = application;
    contacs = new QList<Contact>();

    QMap<QString, QString> *map = new QMap<QString, QString>();
    map->insert("order", "hints");
    map->insert("fields","online");
    QJsonObject response = this->application->getApiMethodExecutor()->executeMethod("friends.get",*map);
}

QList<Contact> Contacts::getAll() const
{
    return contacts;
}
