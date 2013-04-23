#ifndef GITLAB_H
#define GITLAB_H

#include <QMap>
#include <QStringList>
#include <QString>
#include <functional>

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "http.h"

namespace GitLab {

#define PREFIX "http://127.0.0.1/api/v3"

class User: public QObject {
    Q_OBJECT
public:
    
    
protected:
    QMap<QString, std::function<int(Map&)> handle;
    Http::Http *http;
};






}
#endif // GITLAB_H
