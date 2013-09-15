#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "contactmodel.h"

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

    void applyContactModel(ContactModel *contactModel);
public slots:
    void showDialog(QModelIndex modelIndex);
private:
    Ui::MainWindow *ui;
    Application *application;
    ContactModel *contactModel;
};

#endif // MAINWINDOW_H
