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
#include <QDebug>


namespace GitLab {

enum class STATUS {
    OK = 200,                                                  /* the GET, PUT, DELETE request wal successful, resources is returned as JSON */
    CREATED = 201,                                     /* the POST request was successful and the resource is returned as JSON */
    BAD_REQUEST = 400,                          /* a required attribute of the API request is missing.eg the title of an issue is not given */
    UNAUTHORIZED = 401,                        /* the user is not authenticated */
    FORBIDDEN = 403,                                 /* request is not allowed */
    NOT_FOUND = 404,                               /* a resource  could not be accessed */
    METHOD_NOT_ALLOWED = 405,      /* the request is not supported */
    CONFLICT = 409,                                    /* a conflicting resource already exists. */
    SERVER_ERROR = 500                          /* while handling the request something went wrong on the server. */
};


const QString VERSION{"3"};
const QString PREFIX {"http://chideat.org/api/v" + VERSION};

class GitLab : public QObject {
    Q_OBJECT
public :
    bool hasToken(const Json &param) {
        return param.contains("private_token") ? true : false;
    }

    void replyAnalyize(QNetworkReply *reply, Json &result)  {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        result = doc.object();
    }

    /**
     * @brief check params in param
     * @param list
     */
//    bool paramCheck(QStringList &list, Map &param) {
//        for(QString &item : list) {
            
//        }
//    }
    
protected:
    Http::Http *http;
Q_SIGNALS:
    void finished(const QString &id, const Json &data, int state = 0, const QString &message = QString());
    void finished(const QString &id, int state = 0, const QString &message = QString());
};


class Session: public GitLab {
    Q_OBJECT
public:
    Session(GitLab *parent = NULL){
        http = new Http::Http(this);
    }

    /**
     * POST /session
     * param
     *         email
     *         password
     */
    bool login(Json &param) {
        const QString id {"session-login"};
        const QString route { "/session"};
        
        if(!param.contains("email") || !param.contains("password")) {
            throw std::runtime_error("login email and password is need");
        }
        
        QNetworkRequest request(QUrl(QString("%1%2").arg(PREFIX).arg(route)));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonDocument data(param);
        QNetworkReply *reply = http->POST(request, data.toJson());
        
        connect(reply, &QNetworkReply::finished, [this, reply, id](){
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            switch((STATUS)statusCode) {
            case STATUS::CREATED:
                Json result;
                replyAnalyize(reply, result);
                emit finished(id, result, 0);
                break;
            default:
                emit finished(id, 1, reply->errorString());
                break;
            }
            reply->deleteLater();
        });
        return 0;
    }
};

class User: public GitLab {
    Q_OBJECT 
    
public:
    User(GitLab *parent = NULL) {
        http = new Http::Http(this);
    }
    
    /**
     *  GET /users
     *  param
     *        page
     *        per_page 
     */
    int getUsers(Map &param) {
        const QString route = "/users";
        
        
        
        return 0;
    }
    
    /**
     * GET /users/:id
     * param 
     *        id
     */
    int getUser(Map &param) {
        const QString route = "/users/:id";
        
        return 0;
    }
    
    /**
      * POST /users
      * param 
      *        email
      *        password
      *        username
      *        name 
      *        projects_limit(opt)
      */
    int createUser(Map &param) {
        const QString route = "/users";
        
        return 0;
    }
    
    /**
      * PUT /users/:id
      * param
      *        email
      *        username
      *        name
      *        password
      *        projects_limit
      */
    bool updateUser(Json &param, QByteArray token) {
        const QString id="user-updateUser";
        
        if(!param.contains("id")) {
            throw std::runtime_error("user id is needed");
        }
        const QString route = QString("/users/%1").arg(param["id"].toString());
        
        if(token.isNull() || token.isEmpty()) {
            if(!param.contains("private_token"))
                throw std::runtime_error("private token must provided");
            else {
                token = param["private_token"].toString().toLatin1();
                param.remove("private_token");
            }
        }
        
        if(param.keys().length() <= 2)
            return true;
        
        QNetworkRequest request(QUrl(QString("%1%2").arg(PREFIX).arg(route)));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader("PRIVATE-TOKEN", token);

        QJsonDocument data(param);
        QNetworkReply *reply = http->PUT(request, data.toJson());
        
        connect(reply, &QNetworkReply::finished, [this, reply, id](){
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            switch((STATUS)statusCode) {
            case STATUS::OK:
                Json result;
                replyAnalyize(reply, result);
                emit finished(id, result, 0);
                break;
            case STATUS::UNAUTHORIZED:
                emit finished(id, 1, reply->errorString());
            default:
                emit finished(id, 1, reply->errorString());
                break;
            }
            
            reply->deleteLater();
        });
        
        return 0;
    }
    
    /**
     * DELETE /users/:id
     * param
     *        id
     */
    int delUser(Map &param) {
        const QString route = "/users/:id";
        
        
        return 0;
    }
    
    /**
      * GET /user
      * param
      *        token
      */
    int currentUser(Map &param) {
        const QString route = "/user";
        
        
        return 0;
    }
    
    /**
      * GET /user/keys
      * param
      *        token
      */
    int getKeys(Map &param) {
        const QString route = "/user/keys";
        
        return 0;
    }
    
    /**
     * GET /user/keys/:id
     * param
     *         id
     */
    int getKey(Map &param) {
        const QString route = "/user/keys/:id";
        
        return 0;
    }
    
    /**
      * POST /user/keys
      * param
      *         title
      *         key
      */
    int addKey(Json &param, QByteArray token) {
        const QString id{"user-addkey"};
        const QString route {"/user/keys"};
        if(!param.contains("key")) {
            throw std::runtime_error("a public key is needed");
        }
        if(token.isNull() || token.isEmpty()) {
            if(!param.contains("private_token"))
                throw std::runtime_error("private token must provided");
            else {
                token = param["private_token"].toString().toLatin1();
                param.remove("private_token");
            }
        }
        
        if(!param.contains("title")) { 
            QJsonValue title{"Qt5 gitlab lib"};
            param.insert("title", title);
        }

        QNetworkRequest request(QUrl(QString("%1%2").arg(PREFIX).arg(route)));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader("PRIVATE-TOKEN", token);
        
        QJsonDocument data(obj);
        QNetworkReply *reply = http->POST(request, data.toJson());

        connect(reply, &QNetworkReply::finished, [this, reply, id](){
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            switch((STATUS)statusCode) {
            case STATUS::CREATED:
                //replyAnalyize(reply, result);
                emit finished(id, 0, "");
                break;
            case STATUS::NOT_FOUND :
                emit finished(id, 1, "this key has exists");
                break;
            case STATUS::UNAUTHORIZED:
                emit finished(id, 1, "not authorized");
            default:
                emit finished(id, 1, reply->errorString());
                break;
            }
            reply->deleteLater();
        });
        return 0;
    }
    
    /**
      * POST /usrs/:id/keys
      * param
      *        id
      *        title
      *        key
      */
    int addKeyFor(Map &param) {
        const QString route = "/users/:id/keys";
        
        return 0;
    }
    
    /**
      * DELETE /user/keys/:id
      * param
      *         id
      */
    int delKey(Map &param)  {
        const QString route = "/user/keys/:id";
        
        return 0;
    }
    
};




}
#endif // GITLAB_H
