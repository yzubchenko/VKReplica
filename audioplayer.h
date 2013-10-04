#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "application.h"
#include "audiomodel.h"

#include <QDialog>
#include <QMediaPlayer>

class Application;
namespace Ui {
class AudioPlayer;
}

class AudioPlayer : public QDialog {
    Q_OBJECT

public:
    AudioPlayer(const Application* application, QWidget *parent = 0);
    ~AudioPlayer();
    const QIcon playIcon = QIcon(":/audioplayer/resources/playControl.png");
    const QIcon pauseIcon = QIcon(":/audioplayer/resources/pauseControl.png");
public slots:
    void onSearchButtonClicked();
    void onPrevious() const;
    void onPlay() const;
    void onNext() const;
    void onSetVolume(int volume) const;
signals:
    void searchRequest(QString request);
    void stateChanged(QMediaPlayer::State);
private:
    Ui::AudioPlayer* ui;
    const Application* application;
    AudioModel* audioModel;
    QMediaPlayer* player;
    bool isPlayingOnMouseHold;

private slots:
    void onRowDoubleClicked(QModelIndex modelIdx) const;
    void onDurationChanged(qint64 duration) const;
    void onPositionChanged(qint64 pos) const;
    void onCurrentAudioIndexChanged(int index) const;
    void onStateChanged(QMediaPlayer::State state);
    void onMouseReleasedOnValue(uint pos) const;
    void onMousePressed();
};

#endif // AUDIOPLAYER_H
