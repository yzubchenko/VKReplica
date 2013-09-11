#ifndef CONTACTS_H
#define CONTACTS_H

#include <QObject>
#include "application.h"
#include <QList>
#include <QAbstractListModel>

struct Contact {
    QString userId;
    int rating;
    QString displayName;
    bool isOnline;
    bool unreadMessage;
};

Q_DECLARE_METATYPE(Contact *)

class ContactModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum SortOrder {
        DescRating,
        AscDisplayName
    };


    explicit ContactModel(Application *application, QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    QList<Contact*>* getAll() const;
    Contact* getByRow(int row);
    Contact* findByUserId(QString userId);


private:
    Application* application;
    QList<Contact*>* contactList;
    QList<QString>* orderList;
    SortOrder sortOrder = DescRating;

    void insert(Contact *contact, int index = -1);
signals:

public slots:
    void setContactOnline(QString userId, bool isOnline);
    void acceptHasUnreadMessage(QString userId, bool hasUnread);
};

#endif // CONTACTS_H
