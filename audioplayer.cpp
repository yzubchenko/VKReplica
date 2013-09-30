#include "audiodelegate.h"
#include "audioplayer.h"
#include "ui_audioplayer.h"

#include <QJsonObject>

AudioPlayer::AudioPlayer(const Application* application, QWidget *parent) : QDialog(parent), ui(new Ui::AudioPlayer) {
    this->application = application;
    this->audioModel = new AudioModel(application,this);
    connect(this, SIGNAL(searchRequest(QString)), audioModel, SLOT(search(QString)));
    ui->setupUi(this);
    connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(onSearchButtonClicked()));
    ui->listView->setModel(audioModel);
    ui->listView->setItemDelegate(new AudioDelegate());
}

AudioPlayer::~AudioPlayer() {
    delete ui;
}

void AudioPlayer::onSearchButtonClicked() {
    if (!ui->lineEdit->text().isEmpty()) {
        emit searchRequest(ui->lineEdit->text());
    }

}
