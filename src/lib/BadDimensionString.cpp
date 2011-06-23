//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Self
#include "BadDimensionString.h"

BadDimensionString::BadDimensionString() throw ()
{
}

BadDimensionString::~BadDimensionString() throw ()
{
}

const char* BadDimensionString::what() const throw ()
{
    QString what("Given the dimension string %1 which is unknown.");
    std::string whatString = what.arg(m_string).toStdString();
    return whatString.c_str();
}

void BadDimensionString::setString(QString string) throw ()
{
    m_string = string;
}

QString BadDimensionString::string() const throw ()
{
    return m_string;
}
