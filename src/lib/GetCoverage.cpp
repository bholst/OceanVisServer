//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#include "GetCoverage.h"

GetCoverage::GetCoverage()
    : RequestBase(RequestBase::WCS)
{
}

GetCoverage::~GetCoverage()
{
}

QString GetCoverage::request() const
{
    return QString("GetCoverage");
}
    
QList<DimensionSubset> GetCoverage::dimensionSubsets() const
{
    return m_dimensionSubsets;
}

void GetCoverage::setDimensionSubsets(const QList<DimensionSubset> &dimensionSubsets)
{
    m_dimensionSubsets = dimensionSubsets;
}

QString GetCoverage::coverageId() const
{
    return m_coverageId;
}

void GetCoverage::setCoverageId(const QString& coverageId)
{
    m_coverageId = coverageId;
}
