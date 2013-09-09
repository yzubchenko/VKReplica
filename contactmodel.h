#ifndef CONTACTS_H
#define CONTACTS_H

#include <QObject>
#include "application.h"
#include <QList>
#include <QAbstractListModel>

struct Contact {
    QString userId;
    QString displayName;
    bool isOnline;
    bool unreadMessage;
};

class ContactModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ContactModel(Application *application, QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    QList<Contact*>* getAll() const;

    void setHasUnreadMessage(QString user_id, bool isRead);
private:
    Application* application;
    QList<Contact*>* contactList;
    QList<QString>* orderList;

signals:

public slots:

};

#endif // CONTACTS_H
