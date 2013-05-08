#ifndef GITX_H
#define GITX_H

#include <QObject>
#include "utils.h"

class GitX : public QObject {
    Q_OBJECT
    
public:
    
public Q_SLOTS:
    virtual bool login(Map &param) = 0;
    //virtual bool sshkey() = 0;
    virtual bool createSshKey(Map &param) = 0;
    virtual bool addSshKey(Map &param) = 0;
    virtual bool createRepos(Map &param) = 0;
    virtual bool getRepos() = 0;
    
    virtual bool finished() = 0;
    virtual bool parse(Map &param) = 0;
};

#endif // GITX_H
