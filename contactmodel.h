#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QObject>
#include <QList>
#include <QSet>
#include <QAbstractListModel>

class Application;

struct Contact {
    QString userId;
    int rating;
    QString displayName;
    bool isOnline;
    bool hasUnreadMessage;
};

Q_DECLARE_METATYPE(Contact *)

class ContactModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum SortOrder {
        DescRating,
        AscDisplayName
    };

    ContactModel(const Application* application, QObject *parent = 0);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    Contact* getByRow(int row) const;
    Contact* findByUserId(const QString& userId) const;
    void applyContactsVisibility(const bool allVisible);


    void push(Contact* contact);
    void refreshContact(Contact* contact);
    void load();
    void reloadFromStorage();
    void unload();

public slots:
    void setContactOnline(QString userId, bool isOnline);
    void acceptUnreadMessage(QString userId, bool hasUnread);

signals:
    void hasUnreadMessage(bool);
private:
    const Application* application;
    QList<Contact*> contactList;
    QList<Contact*> contactStorage;
    QSet<QString> unreadSet;
    SortOrder sortOrder = DescRating;

    bool allVisible;
    void sort();
    void insert(Contact* contact, int index = -1);
    void checkUnreadMessages();

};


#endif // CONTACTMODEL_H
