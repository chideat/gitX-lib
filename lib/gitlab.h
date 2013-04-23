#ifndef GITLAB_H
#define GITLAB_H

#include <QMap>
#include <QStringList>
#include <QString>

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "http.h"

namespace GitLab {

#define VERSION "3"
#define PREFIX "http://chideat.org/api/v" VERSION

class User: public QObject {
    Q_OBJECT
public:
    
    
    
protected:
    Http::Http *http;
};






}
#endif // GITLAB_H
