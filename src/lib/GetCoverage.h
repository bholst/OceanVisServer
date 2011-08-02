//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef GETCOVERAGE_H
#define GETCOVERAGE_H

// Qt
#include <QtCore/QList>
#include <QtCore/QSize>
#include <QtCore/QMap>

// Project
#include "global.h"
#include "RequestBase.h"
#include "DimensionSubset.h"
#include "DataLayer.h"

class GetCoverage : public RequestBase
{
public:
    GetCoverage(RequestBase::RequestType type = RequestBase::OVP);
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
    
    /**
     * Set the cut mode.
     * Default: Contains
     */
    void setCutMode(DataLayer::CutMode mode);
    
    /**
     * Return the cut mode.
     */
    DataLayer::CutMode cutMode() const;
    
    /**
     * Set the size of the resulting image.
     * Has no effect if the format is text/xml.
     */
    void setSize(Dimension dimension, int size);
    
    int size(Dimension dimension) const;
    
    QMap<Dimension, int> sizes() const;
    
    static GetCoverage *fromRequestString(QString request, RequestBase::RequestType type = RequestBase::OVP);
    
    QString toRequestString(const QString& url) const;
    
private:
    QList<DimensionSubset*> m_dimensionSubsets;
    QString m_coverageId;
    QString m_format;
    DataLayer::CutMode m_cutMode;
    QMap<Dimension, int> m_sizes;
};

#endif
