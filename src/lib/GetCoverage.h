//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef GETCOVERAGE_H
#define GETCOVERAGE_H

// Qt
#include <QtCore/QList>

// Project
#include "RequestBase.h"
#include "DimensionSubset.h"

class GetCoverage : public RequestBase
{
public:
    GetCoverage();
    virtual ~GetCoverage();
    
    virtual QString request() const;
    
    QList<DimensionSubset> dimensionSubsets() const;
    void setDimensionSubsets(const QList<DimensionSubset> &dimensionSubsets);
    
    QString coverageId() const;
    void setCoverageId(const QString& coverageId);
    
private:
    QList<DimensionSubset> m_dimensionSubsets;
    QString m_coverageId;
};

#endif
