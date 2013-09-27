#include <QApplication>
#include "application.h"



int main(int argc, char* argv[])
{
    QApplication qApplication(argc, argv);
    Application* application = new Application();

    application->exec();

    return qApplication.exec();
}
