#ifndef GITLAB_TEST_H
#define GITLAB_TEST_H

#include "../gitx.h"
#include "../gitlab.h"
#include <QMap>

namespace Test {

class TGitLab : public GitX {
    Q_OBJECT
public:
    TGitLab(GitX *parent = NULL);
    ~TGitLab();
public Q_SLOTS:
    bool login(Map &param);
    bool createSshKey(Map &param);
    bool addSshKey(Map &param);
    bool createRepo(Map &param);
    bool getRepos();
    bool createPro(Map &param);
    bool getPros();
    
    bool parse(Map &param) {}
    bool finished() {}
    
protected:
    GitLab::Session *session;
    GitLab::User *user;
    
    QByteArray username;
    QByteArray password;
    QByteArray token;
    
};






}


#endif // TEST_H
