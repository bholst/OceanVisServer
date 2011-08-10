//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Project
#include "Constant.h"
#include "CoordinateAxis.h"

// Self
#include "Coverage.h"

Coverage::Coverage()
    : m_minValue(0.0),
      m_maxValue(0.0)
{
}

Coverage::~Coverage()
{
}

void Coverage::setCoverageId(const QString& coverageId)
{
    m_coverageId = coverageId;
}

QString Coverage::coverageId() const
{
    return m_coverageId;
}

void Coverage::setConstants(const QList<Constant>& constants)
{
    m_constants = constants;
}

QList<Constant> Coverage::constants() const
{
    return m_constants;
}

void Coverage::setCoordinateAxes(const QList<CoordinateAxis>& axes)
{
    m_axes = axes;
}

QList<CoordinateAxis> Coverage::coordinateAxes() const
{
    return m_axes;
}

void Coverage::setColorMap(const ColorMap& colorMap)
{
    m_colorMap = colorMap;
}

ColorMap Coverage::colorMap() const
{
    return m_colorMap;
}

void Coverage::setMaxValue(double maxValue)
{
    m_maxValue = maxValue;
}

double Coverage::maxValue() const
{
    return m_maxValue;
}

void Coverage::setMinValue(double minValue)
{
    m_minValue = minValue;
}

double Coverage::minValue() const
{
    return m_minValue;
}


