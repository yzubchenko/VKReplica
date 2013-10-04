#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QAction>
#include <QSystemTrayIcon>
#include <QMenu>

class Application;

class SystemTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    SystemTrayIcon(Application* application, QObject *parent = 0);
    ~SystemTrayIcon();
    void setupContextMenu();
signals:

public slots:
    void onActivated(QSystemTrayIcon::ActivationReason activationReason);
    void onContextActionTriggered(QAction* action);
private:
    Application* application;
    QMenu* contextMenu;
    QAction* exitAction;


};

#endif // SYSTEMTRAYICON_H
