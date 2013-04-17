#ifndef KEYRING_H
#define KEYRING_H

#include <QObject>
#include <QDBusInterface>



class Keyring : public QObject {
    Q_OBJECT
public :
    Keyring(QObject *parent = NULL):QObject(parent) {   
    }
};

#endif // KEYRING_H
