#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCoreApplication>
#include "lib/gitlab/test.h"

using namespace Test;

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    
    TGitLab gitlab;
    Map param;
    //signin
    param.insert("email", "chinaxiahaifeng@gmail.com");
    param.insert("password", "china123");
    gitlab.login(param);
    
    param.clear();



    return app.exec();
}