//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Self
#include "BoundingBox.h"

BoundingBox::BoundingBox()
    : m_lonMin(0.0),
      m_lonMax(0.0),
      m_latMin(0.0),
      m_latMax(0.0)
{
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::setLonMin(double lonMin)
{
    m_lonMin = lonMin;
}

double BoundingBox::lonMin() const
{
    return m_lonMin;
}

void BoundingBox::setLonMax(double lonMax)
{
    m_lonMax = lonMax;
}

double BoundingBox::lonMax() const
{
    return m_lonMax;
}

void BoundingBox::setLatMin(double latMin)
{
    m_latMin = latMin;
}

double BoundingBox::latMin() const
{
    return m_latMin;
}

void BoundingBox::setLatMax(double latMax)
{
    m_latMax = latMax;
}

double BoundingBox::latMax() const
{
    return m_latMax;
}
