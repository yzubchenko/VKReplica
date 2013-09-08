#ifndef CONTACTS_H
#define CONTACTS_H

#include <QObject>
#include "application.h"

class Contacts : public QObject
{
    Q_OBJECT
public:
    explicit Contacts(Application *application, QObject *parent = 0);
private:
    Application* application;
signals:

public slots:

};

#endif // CONTACTS_H
