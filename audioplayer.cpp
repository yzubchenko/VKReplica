#include "audiodelegate.h"
#include "audioplayer.h"
#include "ui_audioplayer.h"

#include <QJsonObject>
#include <QMediaPlaylist>

AudioPlayer::AudioPlayer(const Application* application, QWidget *parent) : QDialog(parent), ui(new Ui::AudioPlayer) {
    this->application = application;
    this->audioModel = new AudioModel(application,this);
    connect(this, SIGNAL(searchRequest(QString)), audioModel, SLOT(search(QString)));
    ui->setupUi(this);
    connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(onSearchButtonClicked()));
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onRowDoubleClicked(QModelIndex)));

    ui->listView->setModel(audioModel);
    ui->listView->setItemDelegate(new AudioDelegate());
    player = new QMediaPlayer(this,QMediaPlayer::StreamPlayback);
    player->setPlaylist(audioModel->getPlaylist());
    connect(player->playlist(), SIGNAL(currentIndexChanged(int)), audioModel, SLOT(refreshCurrentPlaying(int)));
    connect(player->playlist(), SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentAudioIndexChanged(int)));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChanged(qint64)));
}

AudioPlayer::~AudioPlayer() {
    delete ui;
}

void AudioPlayer::onSearchButtonClicked() {
    if (!ui->lineEdit->text().isEmpty()) {
        emit searchRequest(ui->lineEdit->text());
    }
}

void AudioPlayer::onRowDoubleClicked(QModelIndex modelIdx) const {
    player->playlist()->setCurrentIndex(modelIdx.row());
    player->play();
}

void AudioPlayer::onPositionChanged(qint64 pos) const {
    ui->elapsedBar->setValue((int)(pos/1000));
}

void AudioPlayer::onCurrentAudioIndexChanged(int index) const {
    ui->elapsedBar->setRange(0,audioModel->getByRow(index)->duration);
    ui->elapsedBar->setFormat(audioModel->getByRow(index)->displayName);
}

