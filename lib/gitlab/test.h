#ifndef GITLAB_TEST_H
#define GITLAB_TEST_H

#include "../gitx.h"
#include <QMap>

namespace Test {

class GitLab : public GitX {
    Q_OBJECT
public:
    
public Q_SLOTS:
    bool login(Map &param);
    bool createSshKey(Map &param);
    bool addSshKey(Map &param);
    bool createRepos(Map &param);
    bool getRepos();
    
    
    
};






}


#endif // TEST_H
