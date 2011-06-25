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
    
    QList<DimensionSubset*> dimensionSubsets() const;

    /**
     * Set the dimension subsets.
     * The GetCoverage object takes the ownership of the DimensionSubset
     * objects.
     */
    void setDimensionSubsets(const QList<DimensionSubset*> &dimensionSubsets);

    /**
     * Add a dimension subset.
     * The GetCoverage object takes the ownership of the DimensionSubset
     * object.
     */
    void addDimensionSubset(DimensionSubset* dimensionSubset);
    
    QString coverageId() const;
    void setCoverageId(const QString& coverageId);
    
    /**
     * Set the return format. Possible formats are for example:
     * text/xml
     * image/png
     */
    void setFormat(const QString& format);
    
    /**
     * Return the return format.
     */
    QString format() const;
    
private:
    QList<DimensionSubset*> m_dimensionSubsets;
    QString m_coverageId;
    QString m_format;
};

#endif
