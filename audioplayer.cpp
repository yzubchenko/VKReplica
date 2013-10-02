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
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(onDurationChanged(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChanged(qint64)));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onStateChanged(QMediaPlayer::State)));

    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(onPlay()));
    connect(ui->prevButton, SIGNAL(clicked()), this, SLOT(onPrevious()));
    connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(onNext()));
    connect(ui->volumeControl, SIGNAL(valueChanged(int)), this, SLOT(onSetVolume(int)));
    connect(ui->elapsedBar, SIGNAL(mouseReleasedOnValue(uint)), this, SLOT(onMouseReleasedOnValue(uint)));
    connect(ui->elapsedBar, SIGNAL(mousePressedOnValue(uint)), this, SLOT(onMousePressed()));
}

AudioPlayer::~AudioPlayer() {
    delete ui;
}

void AudioPlayer::onSearchButtonClicked() {
    if (!ui->lineEdit->text().isEmpty()) {
        emit searchRequest(ui->lineEdit->text());
    }
}

void AudioPlayer::onPrevious() const {
    if (player->position() > 3000) { //если больше 3 секунд от начала прослушивания
        onMouseReleasedOnValue(0);   //устанавливаем позицию на начало
    } else {
        player->playlist()->previous();
    }
}

void AudioPlayer::onNext() const {
    player->playlist()->next();
}

void AudioPlayer::onSetVolume(int volume) const {
    player->setVolume(volume);
}

void AudioPlayer::onRowDoubleClicked(QModelIndex modelIdx) const {
    player->playlist()->setCurrentIndex(modelIdx.row());
    player->play();
}

void AudioPlayer::onDurationChanged(qint64 duration) const {
    ui->elapsedBar->setRange(0,duration);
}

void AudioPlayer::onPositionChanged(qint64 pos) const {
    ui->elapsedBar->setValue(pos);
    QString value = QString(" [").append(QString::number((int)(pos/1000))).append(" : ").append(QString::number((int)(ui->elapsedBar->maximum()/1000))).append("]");
    ui->elapsedBar->setValueText(value);

}

void AudioPlayer::onCurrentAudioIndexChanged(int index) const {
    ui->elapsedBar->setBaseText(audioModel->getByRow(index)->displayName);
}

void AudioPlayer::onStateChanged(QMediaPlayer::State state) {
    switch (state) {
        case QMediaPlayer::PlayingState:{
            ui->playButton->setIcon(pauseIcon);
            break;
        }
        case QMediaPlayer::StoppedState:
        case QMediaPlayer::PausedState: {
            ui->playButton->setIcon(playIcon);
            break;
        }
    }
    emit stateChanged(state);
}

void AudioPlayer::onPlay() const {
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
    } else {
        player->play();
    }
}

void AudioPlayer::onMousePressed() {
    isPlayingOnMouseHold = player->state() == QMediaPlayer::PlayingState;
    player->pause();
}


void AudioPlayer::onMouseReleasedOnValue(uint pos) const {
    player->setPosition(pos);
    if (isPlayingOnMouseHold) {
        player->play();
    }

}


