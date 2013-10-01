#ifndef AUDIOMODEL_H
#define AUDIOMODEL_H

#include "application.h"

#include <QAbstractListModel>
#include <QMediaPlaylist>
#include <QObject>

struct Audio {
    QString id;
    QString displayName;
    unsigned long duration;
    QString url;
    bool isPlaying;
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
    Audio* getByRow(int row) const;
    QMediaPlaylist* getPlaylist() const {return playlist;}
signals:

public slots:   
    void search(QString message);
    void refreshCurrentPlaying(int currentPlaying);
private:
    const Application* application;
    QList<Audio*> audioList;
    QMediaPlaylist* playlist;
    int currentPlaying;

};

#endif // AUDIOMODEL_H
