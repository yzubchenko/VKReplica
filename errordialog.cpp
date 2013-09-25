#include "errordialog.h"
#include "ui_errordialog.h"

ErrorDialog::ErrorDialog(QString text, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorDialog) {
    ui->setupUi(this);
    ui->label->setText(text);

}

ErrorDialog::~ErrorDialog()
{
    delete ui;
}
