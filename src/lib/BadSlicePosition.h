//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef BADSLICEPOSITION_H
#define BADSLICEPOSITION_H

// STD
#include <exception>

// Project
#include "global.h"
#include "BadDimensionTypeException.h"

class QVariant;

class BadSlicePosition : public std::exception
{
public:
    BadSlicePosition(Dimension dimension) throw ();
    virtual ~BadSlicePosition() throw ();

    virtual const char* what() const throw ();
    
    Dimension dimension() const throw ();
    
    void setGivenValue(const QVariant& givenValue) throw (BadDimensionTypeException);
    QVariant givenValue() const throw ();
    
    void setLowerLimit(const QVariant& lowerLimit) throw (BadDimensionTypeException);
    QVariant lowerLimit() const throw();
    
    void setUpperLimit(const QVariant& upperLimit) throw (BadDimensionTypeException);
    QVariant upperLimit() const throw();

private:
    Dimension m_dimension;
    QVariant m_givenValue;
    QVariant m_lowerLimit;
    QVariant m_upperLimit;
};

#endif
