//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <string>

// Qt
#include <QtCore/QString>

// Self
#include "BadDimensionTypeException.h"

BadDimensionTypeException::BadDimensionTypeException() throw ()
    : m_dimension(Time),
      m_expectedType(QVariant::Invalid),
      m_givenType(QVariant::Invalid)
{
}

const char* BadDimensionTypeException::what() const throw ()
{
    QString what("Given object of type %1 instead of type %2 for dimension %3");
    std::string whatString = what.arg(m_givenType).arg(m_expectedType).arg(m_dimension).toStdString();
    return whatString.c_str();
}

void BadDimensionTypeException::setDimension(Dimension dimension) throw ()
{
    m_dimension = dimension;
}

Dimension BadDimensionTypeException::dimension() const throw ()
{
    return m_dimension;
}

void BadDimensionTypeException::setExpectedType(QVariant::Type expectedType) throw ()
{
    m_expectedType = expectedType;
}

QVariant::Type BadDimensionTypeException::expectedType() const throw ()
{
    return m_expectedType;
}

void BadDimensionTypeException::setGivenType(QVariant::Type givenType) throw ()
{
    m_givenType = givenType;
}

QVariant::Type BadDimensionTypeException::givenType() const throw ()
{
    return m_givenType;
}
