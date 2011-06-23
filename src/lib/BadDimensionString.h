//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef BADDIMENSIONSTRING_H
#define BADDIMENSIONSTRING_H

// STD
#include <exception>

// Qt
#include <QtCore/QVariant>

// Project
#include "global.h"

class BadDimensionString : public std::exception
{
public:
    BadDimensionString() throw ();
    virtual ~BadDimensionString() throw ();

    virtual const char* what() const throw ();

    void setString(QString string) throw ();
    QString string() const throw ();

private:
    QString m_string;
};

#endif
