#ifndef HTTP_H
#define HTTP_H

#include <QUrl>
#include <QString>
#include <QVariant>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include "utils.h"

/**
 *  Http module design
 *  
 */

namespace Http {

class Http : public QObject {
    Q_OBJECT
public :
    enum Action {
        A_GET = 1,
        A_POST,
        A_DELETE,
        A_PATCH
    };
    
    Http(QObject *parent = NULL):QObject(parent) {
        manager = new QNetworkAccessManager(parent);
    }
    
    ~Http(){
        delete manager;
    }
    
    QNetworkReply * GET(QNetworkRequest &request) {
        return manager->get(request);
    }
    
    QNetworkReply * POST(QNetworkRequest &request, const QByteArray &data) {
        return manager->post(request, data);
    }
    
    QNetworkReply * PUT(QNetworkRequest &request, const QByteArray &data) {
        return manager->put(request, data);
    }
    
    QNetworkReply * PATCH(QNetworkRequest &request, const QByteArray &data) {
        return manager->post(request, data);
    }
    
    QNetworkReply * DELETE(QNetworkRequest &request) {
        return manager->deleteResource(request);
    }
    
protected :
    QNetworkAccessManager * manager;
};

}

#endif // HTTP_H
