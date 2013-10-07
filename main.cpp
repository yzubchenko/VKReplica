#include <QApplication>
#include <QMessageBox>
#include <QSharedMemory>
#include "application.h"



int main(int argc, char* argv[]) {
    QApplication qApplication(argc, argv);
    QSharedMemory sharedMemory;
    sharedMemory.setKey("VK Replica");
    sharedMemory.attach();
    if (!sharedMemory.create(1)) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("VK Replica");
        msgBox.setText("Невозможно запустить программу, т.к. она уже запущена.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return 1; // Выходим, приложение уже запущено
    } else {
        Application* application = new Application(&qApplication, nullptr);
        application->exec();
        return qApplication.exec();
    }
}
