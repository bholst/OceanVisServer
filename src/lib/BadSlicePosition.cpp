//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QVariant>

// Project
#include "Dimension.h"

// Self
#include "BadSlicePosition.h"

BadSlicePosition::BadSlicePosition(Dimension dimension) throw ()
    : m_dimension(dimension)
{
}

BadSlicePosition::~BadSlicePosition() throw ()
{
}

const char* BadSlicePosition::what() const throw ()
{
    QString what("Slice point out of axis bounds.");
    std::string whatString = what.toStdString();
    return whatString.c_str();
}

Dimension BadSlicePosition::dimension() const throw ()
{
    return m_dimension;
}

void BadSlicePosition::setGivenValue(const QVariant& givenValue) throw (BadDimensionTypeException)
{
    checkDimensionType(m_dimension, givenValue);
    m_givenValue = givenValue;
}

QVariant BadSlicePosition::givenValue() const throw ()
{
    return m_givenValue;
}

void BadSlicePosition::setLowerLimit(const QVariant& lowerLimit) throw (BadDimensionTypeException)
{
    checkDimensionType(m_dimension, lowerLimit);
    m_lowerLimit = lowerLimit;
}
    
QVariant BadSlicePosition::lowerLimit() const throw()
{
    return m_lowerLimit;
}

void BadSlicePosition::setUpperLimit(const QVariant& upperLimit) throw (BadDimensionTypeException)
{
    checkDimensionType(m_dimension, upperLimit);
    m_upperLimit = upperLimit;
}

QVariant BadSlicePosition::upperLimit() const throw()
{
    return m_upperLimit;
}
