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

#define VERSION "3"
#define PREFIX "http://chideat.org/api/v" VERSION

class User: public QObject {
    Q_OBJECT
public:
    
    
    
protected:
    Http::Http *http;
};

class Session: public QObject {
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
            
            if(statusCode == (int)STATUS::CREATED) {
                replyAnalyize(reply, param);
                status(param, 0);
            }
            else if(statusCode == (int)STATUS::CONFLICT) {
               // param["title"] = 
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
    
    bool hasToken(Map &param) {
        if(param.contains("token"))
            return true;
        else 
            return false;
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

protected:
    Http::Http *http;
    
};




}
#endif // GITLAB_H
