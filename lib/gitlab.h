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
        return param.contains("token") ? true : false;
    }

    void replyAnalyize(QNetworkReply *reply, Map &param)  {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject data = doc.object();
        
        for(QByteArray key : param.keys()) {
            if(data.contains(QString(key))) 
                param[key] = data[key].toString();
            else if(reply->rawHeaderList().contains(key)) 
                param[key] = reply->rawHeader(key);
        }
    }

    
    void status(Map &param, int val = 0) {
        if(param.contains("_state")) 
            param["_state_"] = val;
        else
            param["_state"] = val;
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
    void finished();
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
            param["_status"] = 1;
            param["_message"] = "login email and password is need";
            return 1;
        }
        
        QJsonValue email(param["email"].toString());
        QJsonValue password(param["password"].toString());
        QJsonObject obj;
        obj.insert("email", email);
        obj.insert("password", password);
        QJsonDocument data(obj);

        QNetworkRequest request(QUrl(QString("%1%2").arg(PREFIX).arg(route)));
        qDebug()<<QString("%1%2").arg(PREFIX).arg(route)<< "    "<< data.toJson();
        
        if(param.contains("contentTypeHeader")) 
            request.setHeader(QNetworkRequest::ContentTypeHeader, param["contentTypeHeader"]);
        else 
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QNetworkReply *reply = http->POST(request, data.toJson());
        
        connect(reply, &QNetworkReply::finished, [&, reply](){
            if(reply == NULL) {
                qDebug() << "reply is null";
            }
            
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            
            switch((STATUS)statusCode) {
            case STATUS::CREATED:
                replyAnalyize(reply, param);
                status(param);
                break;
            default:
                qDebug()<<"statusCode : "<<reply->errorString();
                
                //status(param, (int)statusCode);
                break;
            }
            emit finished();
            reply->deleteLater();
        });
        return 0;
    }
};

class User: public GitLab {
    Q_OBJECT 
    
public:
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
        const QString route = "/usrs/:id";
        
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
            param["_status"] = 1;
            param["_message"] = "a public key is needed";
            return 1;
        }
        if(!hasToken(param)) {
            param["_status"] = 1;
            param["_message"] = "private token must passed";
            return 2;
        }
        if(!param.contains("title")) 
            param["title"] = "Qt gitlab lib";
        
        QJsonValue title(param["title"].toString());
        QJsonValue key(param["password"].toString());
        QJsonObject obj;
        obj.insert("title", title);
        obj.insert("key", key);
        QJsonDocument data(obj);
        
        QNetworkRequest request(QUrl(QString("%1%2").arg(PREFIX).arg(route)));
        
        if(param.contains("contentTypeHeader")) 
            request.setHeader(QNetworkRequest::ContentTypeHeader, param["contentTypeHeader"]);
        else 
            request.setHeader(QNetworkRequest::ContentTypeHeader, param.contains("application/json"));
        
        request.setRawHeader("PRIVATE-TOKEN", param["token"].toByteArray());
        
        QNetworkReply *reply = http->POST(request, data.toJson());
        connect(reply, &QNetworkReply::finished, [&](){
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            
            switch((STATUS)statusCode) {
            case STATUS::CREATED:
                replyAnalyize(reply, param);
                status(param);
                break;
            default:
                status(param, (int)statusCode);
                break;
            }
            
            emit finished();
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
