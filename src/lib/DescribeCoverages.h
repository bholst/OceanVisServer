//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DESCRIBECOVERAGES_H
#define DESCRIBECOVERAGES_H

// Qt
#include <QtCore/QList>
#include <QtCore/QDateTime>
#include <QtGui/QColor>

// Project
#include "global.h"
#include "RequestBase.h"

class DescribeCoverages : public RequestBase
{
public:
    DescribeCoverages();
    virtual ~DescribeCoverages();
    
    virtual QString request() const;
    
    static DescribeCoverages *fromRequestString(QString request);
    
    QString toString() const;
    
private:
};

#endif
