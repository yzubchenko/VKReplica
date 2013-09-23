#ifndef DIALOGMODEL_H
#define DIALOGMODEL_H


#include <QObject>
#include <QList>
#include <QAbstractListModel>
#include "application.h"

struct Message {
    QString id;
    QString fromDisplayName;
    QString date;
    QString body;
    bool isRead;
};

Q_DECLARE_METATYPE(Message *)

class DialogModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit DialogModel(Application *application, QString userId, QObject *parent = 0);
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

signals:

public slots:

private:
    Application *application;
    QString userId;
    QList<Message*>* messageList;
};

#endif // DIALOGMODEL_H
