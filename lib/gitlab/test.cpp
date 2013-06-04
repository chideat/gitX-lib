#include "test.h"
#include "../gitlab.h"
#include "../notification.h"

#include <exception>
#include <QFile>
#include <QCoreApplication>
#include <QDebug>

using namespace Test;

TGitLab::TGitLab(GitX *parent):GitX(parent) {
    try{
        session = new GitLab::Session;
        user = new GitLab::User;
    }
    catch(const std::exception &e) {
        //_notify(QString("Gitlab Login"), Notification::Error, QString(e.what()));
        qDebug()<<e.what();
        qApp->quit();
    }
    
    /**
    void finished(const QString &id, const Json &data, int state = 0, const QString &message = QString());
    void finished(const QString &id, int state = 0, const QString &message = QString());
      */
    connect(session, &GitLab::Session::finished, [this](QString &id, const Json &data, int state, const QString &message){
        if(!result.contains("id") || result["id"] != "session-login")
            return ;
        // get token
        if(result.contains("status") && result["status"] == 1) {
            //_notify(QString("Gitlab Login"), Notification::Error, QString("login failed"));
            qDebug()<<"login failed";
        }
        else {
            qDebug()<<result["status"].toInt();
        }
        
        currentUser = new User(result["result"].toByteArray(), this);

        
        
        qDebug()<<result["result"].toByteArray();
        
        Map param;
        /*
        //add ssh key
        param.insert("title", "gitlab");
        addSshKey(param);
        */
        
        // update user
        param.insert("name", "china12345");
        updateUser(param);
        
    });
    
}

TGitLab::~TGitLab() {
    delete session;
    delete user;
}

/**
 * @brief GitLab::login
 * @param param {email, password}
 * @return 
 */
bool TGitLab::login(Map &param) {
    // use saved token to automatically sign in
    if(param.isEmpty()) {
        // get tokens or password and email from storage
        
    }
    else if(!param.contains("email") || !param.contains("password")) {
        //notify error
    }
    else if(param.contains("email") && param.contains("password")) {
        //login in with email and password
        
        session->login(param);
        
        
    }
    return true;
}

/**
 * @brief TGitLab::createSshKey 
 * @param param
 * @return 
 */
QByteArray TGitLab::createSshKey(Map &param){
    // generate ssh key
    //read ssh file 
    QFile file("/home/thinking/.ssh/id_rsa.pub");
    file.open(QIODevice::ReadOnly);
    QByteArray sshKey = file.readAll();
    file.close();
    return sshKey;
}

/**
 * @brief TGitLab::addSshKey
 * @param param
 * @return 
 */
bool TGitLab::addSshKey(Map &param){
    if(!param.contains("title")) 
        param.insert("title", "gitlab");
    param.insert("key", createSshKey(param));
    currentUser->setToken(param);
    
    connect(user, &GitLab::User::finished, [this](Map &result){
        if(!result.contains("id") || result["id"] != "user-addkey") 
            return ;
        // get result 
        if(result.contains("status") && result["status"] == 1) {
            //_notify(QString("Gitlab Login"), Notification::Error, QString("login failed"));
            qDebug()<<result["message"].toString();
        }
        else {
            qDebug()<<result["status"].toInt();
        }
    });
    
    user->addKey(param);
    
    //user-addkey
    return true;
}

bool TGitLab::updateUser(Map &param) {
    qDebug()<<"here";
    if(param.isEmpty()) 
        return true;
    currentUser->setToken(param);
    currentUser->setId(param);
    
    connect(user, &GitLab::User::finished, [this](Map &result) {
        qDebug()<<"here";
        if(!result.contains("id") || result["id"] != "user-updateUser") 
            return ;
        //update user info
        if(!result.contains("result"))
            return ;
        QJsonDocument doc = QJsonDocument::fromJson(result["result"].toByteArray());
        QJsonObject obj(doc.object());
        for(QString key: obj.keys()) {
            QVariant val(obj[key].toString());
            currentUser->setProperty(key.toLatin1().data(), val);
        }
    });
    user->updateUser(param);
}

bool TGitLab::createRepo(Map &param){}
bool TGitLab::getRepos(){}
bool TGitLab::createPro(Map &param){}
bool TGitLab::getPros(){}

QString TGitLab::getSercet() {
    
    return "";
}

bool TGitLab::setSercet(Map &param) {
    
}

bool User::temp(Map &param, const QString &key) {
    if(data.contains(key)) {
        if(data[key].isBool()) {
            param.insert(key.toLatin1(), data[key].toBool());
        }
        else if(data[key].isDouble()) {
            param.insert(key.toLatin1(), data[key].toDouble());
        }
        else  {
            param.insert(key.toLatin1(), data[key].toString());
        } 
        
        qDebug() << param[key.toLatin1()];
        return true;
    }
    throw std::runtime_error(QString("key %1 doesn't exists").arg(key).toStdString());
}


bool User::setToken(Map &param) {
    const QString key{"private_token"};
    temp(param, key);
}

bool User::setId(Map &param) {
    const QString key {"id"};
    temp(param, key);
    //qApp->exit();
}

