//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef BADDIMENSIONTYPEEXCEPTION_H
#define BADDIMENSIONTYPEEXCEPTION_H

// STD
#include <exception>

// Qt
#include <QtCore/QVariant>

// Project
#include "global.h"

class BadDimensionTypeException : public std::exception
{
public:
    BadDimensionTypeException() throw ();

    virtual const char* what() const throw ();

    void setDimension(Dimension dimension) throw ();
    Dimension dimension() const throw ();

    void setExpectedType(QVariant::Type expectedType) throw ();
    QVariant::Type expectedType() const throw ();

    void setGivenType(QVariant::Type givenType) throw ();
    QVariant::Type givenType() const throw ();

private:
    Dimension m_dimension;
    QVariant::Type m_expectedType;
    QVariant::Type m_givenType;
    char *m_what;
};

#endif
