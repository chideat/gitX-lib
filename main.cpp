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
    param.insert("email", "chinaxiahaifeng@gmail.com");
    param.insert("password", "chinat123");
    gitlab.login(param);


    return app.exec();
}
