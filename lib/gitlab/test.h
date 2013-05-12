#ifndef GITLAB_TEST_H
#define GITLAB_TEST_H

#include "../gitx.h"
#include "../gitlab.h"
#include <QMap>

namespace Test {

class User : public QObject {
    Q_OBJECT
public:
    User(const QByteArray &&info, QObject *parent = NULL) : QObject(parent) {
        QJsonDocument doc = QJsonDocument::fromJson(info);
        data = doc.object();
    }
public Q_SLOTS:
    
    bool setToken(Map &param);
    bool setId(Map &param);
    bool temp(Map &param, const QString &key);
protected:
    QJsonObject data;
};


class TGitLab : public GitX {
    Q_OBJECT
public:
    TGitLab(GitX *parent = NULL);
    ~TGitLab();
public Q_SLOTS:
    bool login(Json &param);
    QByteArray createSshKey(Map &param);
    bool addSshKey(Map &param);
    bool createRepo(Map &param);
    bool getRepos();
    bool createPro(Map &param);
    bool getPros();
    
    bool parse(Map &param) {}
    bool finished() {}
    bool updateUser(Map &param);
private Q_SLOTS:
    QString getSercet();
    bool setSercet(Map &param); 
    
protected:
    GitLab::Session *session;
    GitLab::User *user;
    
    User *currentUser;
    
};


}


#endif // TEST_H
