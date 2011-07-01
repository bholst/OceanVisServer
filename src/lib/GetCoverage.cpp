//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#include "GetCoverage.h"

GetCoverage::GetCoverage()
    : RequestBase(RequestBase::WCS),
      m_format("text/xml")
{
}

GetCoverage::~GetCoverage()
{
    qDeleteAll(m_dimensionSubsets);
}

QString GetCoverage::request() const
{
    return QString("GetCoverage");
}
    
QList<DimensionSubset*> GetCoverage::dimensionSubsets() const
{
    return m_dimensionSubsets;
}

void GetCoverage::setDimensionSubsets(const QList<DimensionSubset*> &dimensionSubsets)
{
    m_dimensionSubsets = dimensionSubsets;
}

void GetCoverage::addDimensionSubset(DimensionSubset* dimensionSubset)
{
    m_dimensionSubsets.append(dimensionSubset);
}

QString GetCoverage::coverageId() const
{
    return m_coverageId;
}

void GetCoverage::setCoverageId(const QString& coverageId)
{
    m_coverageId = coverageId;
}

void GetCoverage::setFormat(const QString& format)
{
    m_format = format;
}

QString GetCoverage::format() const
{
    return m_format;
}

void GetCoverage::setSize(Dimension dimension, int size)
{
    m_sizes.insert(dimension, size);
}
    
int GetCoverage::size(Dimension dimension) const
{
    return m_sizes.value(dimension);
}

QMap<Dimension, int> GetCoverage::sizes() const
{
    return m_sizes;
}
