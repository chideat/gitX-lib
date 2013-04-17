#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QByteArray>


/**
  * https://api.github.com/authorizations/
  */

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QObject::connect(manager, &QNetworkAccessManager::finished, [=](QNetworkReply *reply) {
        qDebug()<<reply->readAll();
        reply-> deleteLater();
        qApp->quit();
    });
    QNetworkRequest request(QUrl("https://api.github.com/user/keys/4619729"));
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//    QJsonObject param;
//    QJsonValue 
//    param.insert("username", username);
//    param.insert("password", password);

//    QJsonDocument  document(param);
    QByteArray data;
    data.append("{\"title\": \"test1\", \"key\":\"ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDTMfV47zXvbqIlzy1PMqmMsYF1HuFFOBljGhLcYkrH0wah65ob51AFAt+sQcJYYM2uyGb2dUfsAXgYTvUVQ8kuj8ZumD50jJOhrrmKoeSsYjjQRFfIyXtXJbum44NpQ/hlUcwBonlNhbWnBjyf72ZkiBY+nLBa9zaPL0ole2sBOadr1rqFea2HMfEv4MH9dxNR+bczDz3ercYkDIu1ZbaoUm8cFaoHRZPqk5m3AdvEHYBAV74V8zSKsKpUhntYDS0Zv4vyt3hSZVJGOiH5TatoI8MdFx00mHMHVsqziZ5acakgejy0FHmKU8SLXQ3cTnKaVKEZ9SMBYAYcMvYJ5QP3 thinking@chinat\"}");
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg("chinaxiahaifeng@live.com").arg("china123").toLatin1()).toBase64());    
    //request.setRawHeader("Authorization", "token 6f5a2bfc6d03039319bdbdf55523f05d93e861c4");  
    //manager->post(request, data);
    //manager->get(request);
    manager->deleteResource(request);
    return app.exec();
}
