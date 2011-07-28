//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef REQUESTBASE_H
#define REQUESTBASE_H

#include <QtCore/QString>

class RequestBase {
public:
    enum RequestType { 
        WCS,
        WMS,
        OVP
    };
    
    RequestBase(RequestType requestType);
    virtual ~RequestBase();
    
    virtual QString request() const = 0;
    
    RequestBase::RequestType requestType() const;
    
    QString version() const;
    void setVersion(const QString& version);
    
private:
    RequestType m_requestType;
    QString m_version;
};

#endif
