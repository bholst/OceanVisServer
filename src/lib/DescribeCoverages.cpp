//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Project
#include "RequestParser.h"

// Self
#include "DescribeCoverages.h"

DescribeCoverages::DescribeCoverages()
    : RequestBase(RequestBase::OVP)
{
}

DescribeCoverages::~DescribeCoverages()
{
}

QString DescribeCoverages::request() const
{
    return QString("DescribeCoverages");
}

DescribeCoverages *DescribeCoverages::fromRequestString(QString request)
{
    QString version;
    
    QStringList args = request.split('&');
    
    foreach(QString arg, args) {
        QStringList argParts = arg.split('=');
        if(argParts.size() != 2) {
            return 0;
        }
        QString field = argParts[0].toLower();
        QString value = argParts[1];
        
        if(field == "version") {
            version = value;
        }
    }
    
    DescribeCoverages *describeCoverages = new DescribeCoverages();
    describeCoverages->setVersion(version);
    return describeCoverages;
}

QString DescribeCoverages::toString() const
{
    QString result("Getmap (%1):\n"
                   "VERSION=%2\n");
    
    result = result.arg((unsigned long) this, 0, 16);
    result = result.arg(version());
    return result;
}
