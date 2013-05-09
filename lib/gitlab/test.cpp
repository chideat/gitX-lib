#include "test.h"
#include "../gitlab.h"
#include "../notification.h"

#include <exception>
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
    param.insert("token", "");
    
    if(param.isEmpty()) {
        // get tokens or password and email from storage
        
    }
    else if(!param.contains("email") || !param.contains("password")) {
        //notify error
    }
    else if(param.contains("email") && param.contains("password")) {
        //login in with email and password
        connect(session, &GitLab::Session::finished, [&](){
            // get token
            if(param.contains("_status_") && param["_status_"] == 1) {
                //_notify(QString("Gitlab Login"), Notification::Error, QString("login failed"));
                qDebug()<<"login failed";
            }
            else if(param.contains("_status")) {
                qDebug()<<param["_status"].toInt();
            }
            if(param.contains("token") && !param["token"].toString().isEmpty()) {
                qDebug()<<param["token"];
            }
        });
        session->login(param);
    }
    return true;
}



bool TGitLab::createSshKey(Map &param){}
bool TGitLab::addSshKey(Map &param){}
bool TGitLab::createRepo(Map &param){}
bool TGitLab::getRepos(){}
bool TGitLab::createPro(Map &param){}
bool TGitLab::getPros(){}

