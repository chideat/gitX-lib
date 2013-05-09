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
    bool hasToken(Map &param) {
        return param.contains("private_token") ? true : false;
    }

    void replyAnalyize(QNetworkReply *reply, Map &param)  {
        param.insert("result", reply->readAll());
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
    void finished(Map &result);
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
    int login(Map &param) {
        const QString route = "/session";
        
        if(!param.contains("email") || !param.contains("password")) {
            param["status"] = 1;
            param["message"] = "login email and password is need";
            return 1;
        }
        
        QJsonValue email(param["email"].toString());
        QJsonValue password(param["password"].toString());
        QJsonObject obj;
        obj.insert("email", email);
        obj.insert("password", password);
        QJsonDocument data(obj);
        
        QNetworkRequest request(QUrl(QString("%1%2").arg(PREFIX).arg(route)));
        
        if(param.contains("contentTypeHeader")) 
            request.setHeader(QNetworkRequest::ContentTypeHeader, param["contentTypeHeader"]);
        else 
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply *reply = http->POST(request, data.toJson());
        
        connect(reply, &QNetworkReply::finished, [this, reply](){
            Map result;
            result.insert("id", "session-login");
            if(reply == NULL) {
                qDebug() << "reply is null";
            }
            
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            switch((STATUS)statusCode) {
            case STATUS::CREATED:
                replyAnalyize(reply, result);
                result["state"] = 0;
                break;
            default:
                result["state"] = 1;
                break;
            }
            emit finished(result);
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
    int updateUser(Map &param) {
        if(!param.contains("id")) {
            param["state"] = 1;
            param["message"] = "user id is need";
            return 1;
        }
        const QString route = QString("/usrs/%1").arg(param["id"].toString());
        
        if(!hasToken(param)) {
            param["state"] = 1;
            param["message"] = "token id is need";
            return 1;
        }
        if(param.keys().length() <= 2) {
            param["state"] = 0;
            return 0;
        }
        
        QJsonObject obj;
        for(QByteArray key : param.keys()) {
            QJsonValue value(param[key].toString());
            obj.insert(key, value);
        }
        QJsonDocument data(obj);
        
        QNetworkRequest request(QUrl(QString("%1%2").arg(PREFIX).arg(route)));
        
        if(param.contains("contentTypeHeader")) 
            request.setHeader(QNetworkRequest::ContentTypeHeader, param["contentTypeHeader"]);
        else 
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        
        request.setRawHeader("PRIVATE-TOKEN", param["token"].toByteArray());
        
        QNetworkReply *reply = http->PUT(request, data.toJson());
        
        connect(reply, &QNetworkReply::finished, [this, reply](){
            Map result;
            result.insert("id","user-update");
            
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            
            switch((STATUS)statusCode) {
            case STATUS::OK:
                replyAnalyize(reply, result);
                result["state"] = 0;
                break;
            case STATUS::UNAUTHORIZED:
                result["state"] = 1;
                result["message"] = "not authorized";
            default:
                result["state"] = 1;
                result["message"] = reply->errorString();
                break;
            }
            
            emit finished(result);
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
    int addKey(Map &param) {
        const QString route = "/user/keys";
        if(!param.contains("key")) {
            param["status"] = 1;
            param["message"] = "a public key is needed";
            return 1;
        }
        if(!hasToken(param)) {
            param["status"] = 1;
            param["message"] = "private token must provided";
            return 2;
        }
        
        if(!param.contains("title")) 
            param["title"] = "gitlab";

        QJsonValue title(param["title"].toString());
        QJsonValue key(param["key"].toString());
        QJsonObject obj;
        obj.insert("title", title);
        obj.insert("key", key);
        QJsonDocument data(obj);
        
        QNetworkRequest request(QUrl(QString("%1%2").arg(PREFIX).arg(route)));
        
        if(param.contains("contentTypeHeader")) 
            request.setHeader(QNetworkRequest::ContentTypeHeader, param["contentTypeHeader"]);
        else 
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        
        request.setRawHeader("PRIVATE-TOKEN", param["token"].toByteArray());
        
        QNetworkReply *reply = http->POST(request, data.toJson());

        connect(reply, &QNetworkReply::finished, [this, reply](){
            Map result;
            result.insert("id","user-addkey");
            
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            
            switch((STATUS)statusCode) {
            case STATUS::CREATED:
                //replyAnalyize(reply, result);
                result["state"] = 0;
                break;
            case STATUS::NOT_FOUND :
                result["state"] = 1;
                result["message"] = "this key has exists";
                break;
            case STATUS::UNAUTHORIZED:
                result["state"] = 1;
                result["message"] = "not authorized";
            default:
                result["state"] = 1;
                result["message"] = reply->errorString();
                break;
            }
            
            emit finished(result);
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
