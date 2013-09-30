#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "application.h"
#include "audiomodel.h"

#include <QDialog>

class Application;
namespace Ui {
class AudioPlayer;
}

class AudioPlayer : public QDialog {
    Q_OBJECT

public:
    explicit AudioPlayer(const Application* application, QWidget *parent = 0);
    ~AudioPlayer();
public slots:
    void onSearchButtonClicked();
signals:
    void searchRequest(QString request);

private:
    Ui::AudioPlayer* ui;
    const Application* application;
    AudioModel* audioModel;
};

#endif // AUDIOPLAYER_H
