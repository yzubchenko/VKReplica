#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <application.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Application *app, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Application *application;
};

#endif // MAINWINDOW_H
