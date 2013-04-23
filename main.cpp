#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QByteArray>
#include <QStringList>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
/**
  * https://api.github.com/authorizations/
  */


int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QObject::connect(manager, &QNetworkAccessManager::finished, [=](QNetworkReply *reply) {
         int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
         qDebug()<<statusCode;
        QJsonParseError error;
        QJsonDocument doc2 = QJsonDocument::fromJson(reply->readAll(), &error);
        qDebug()<<doc2.toJson();

        reply-> deleteLater();
        qApp->quit();
    }); 
    
    QNetworkRequest request(QUrl("http://chideat.org/api/v3/user/keys"));
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("PRIVATE-TOKEN", "e38WTGZpJqWpWxseZVyD");
    
    //QByteArray data = R"json({"email":"chinaxiahaifeng@gmail.com", "password":"china123"})json";
    QByteArray data = R"json({"title":"test", "key":"ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDXpNlgbh36Lfryc5ftLWYf/tkg/jBzanyAAyDOEBZtukfCsgxsBh87MlTkLyE7f9QT2/TQAB8r1Qx9ZdvkNAFkTAZgkV4BZA9+CiPYv/THZ+KGBCWEnNZcS0+kh7Y3BLmzsMduS/0XkJQF9Wn+v7em5caY3//oCy3iT2sBMOnFSPE9xCqdM+w31sX7TM9sprgjVSoCLfBdlXdoStVn9UsJHxrHTLL0TzeDHWx2Aiqn/2CP2zsEj8fp1VDXVfzNe/0knaHq7WqN5NaCZEpa3EbuimK7i+65gcHS6UfGZ0NMqwn9MA95TLK3zVqvmw62VAj1QEa58XLZh/7y1AlHIYav chinaxiahaifeng@gmail.com"})json";
    manager->post(request, data);
    //manager->get(request);

    return app.exec();
}
