#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dialog) {
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


