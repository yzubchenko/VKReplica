#include "audiomodel.h"

AudioModel::AudioModel(const Application* application, QObject* parent) : QAbstractListModel(parent) {
    this->application = application;
    this->audioList = QList<Audio*>();
}

int AudioModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return audioList.count();
}

QVariant AudioModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= audioList.size()) {
        return QVariant();
    }
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        Audio* audio = audioList.at(index.row());
        QVariant audioVariant;
        audioVariant.setValue(audio);
        return audioVariant;
    }
    return QVariant();
}

void AudioModel::search(QString message) {
    audioList.clear();
    if (!message.isEmpty()) {
        QMap<QString,QString> params;
        params.insert("q",message);
        params.insert("auto_complete","1");
        params.insert("sort","2");
        params.insert("count","200");
        QJsonObject result = application->getApiMethodExecutor().executeMethod("audio.search",params);
        QVariantList items = result.toVariantMap().take("response").toMap().take("items").toList();
        foreach (QVariant item, items) {
            QVariantMap itemMap = item.toMap();
            QString id = itemMap.take("id").toString();
            QString displayName = itemMap.take("artist").toString()
                    .append(" - ")
                    .append(itemMap.take("title").toString());
            unsigned long duration = floor(itemMap.take("duration").toDouble());
            QString url = itemMap.take("url").toString();
            Audio* audio = new Audio{id, displayName, duration, url};
            audioList.push_back(audio);
        }
        int i=0;
    }
    emit dataChanged(index(0), index(audioList.size()-1));

}

bool AudioModel::insertRows(int row, int count, const QModelIndex &parent) {
    if (count < 1 || row < 0 || row > rowCount(parent)) {
        return false;
    }
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int r = 0; r < count; ++r) {
        Audio* emptyAudio = new Audio();
        audioList.insert(row, emptyAudio);
    }
    endInsertRows();
    return true;
}

bool AudioModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (count <= 0 || row < 0 || (row + count) > rowCount(parent)) {
       return false;
    }
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int r = 0; r < count; ++r) {
        audioList.removeAt(row);
    }
    endRemoveRows();
    return true;
}
