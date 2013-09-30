#ifndef AUDIOMODEL_H
#define AUDIOMODEL_H

#include "application.h"

#include <QAbstractListModel>
#include <QObject>

struct Audio {
    QString id;
    QString displayName;
    unsigned long duration;
    QString url;
};

Q_DECLARE_METATYPE(Audio*)

class AudioModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit AudioModel(const Application* application, QObject* parent = 0);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
signals:

public slots:   
    void search(QString message);
private:
    const Application* application;
    QList<Audio*> audioList;

};

#endif // AUDIOMODEL_H
