//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDateTime>
#include <QtCore/QVariant>

// Self
#include "Dimension.h"

void checkDimensionType(Dimension dimension, const QVariant& value) throw (BadDimensionTypeException)
{
    if(dimension == Lon
       || dimension == Lat
       || dimension == Height)
    {
        if(!value.canConvert(QVariant::Double)) {
            BadDimensionTypeException exception;
            exception.setDimension(dimension);
            exception.setExpectedType(QVariant::Double);
            exception.setGivenType(value.type());
            throw exception;
        }
    }
    else if(dimension == Time) {
        if(!value.canConvert(QVariant::DateTime)) {
            BadDimensionTypeException exception;
            exception.setDimension(dimension);
            exception.setExpectedType(QVariant::DateTime);
            exception.setGivenType(value.type());
            throw exception;
        }
    }
}

Dimension dimensionFromString(QString dimension) throw (BadDimensionString)
{
    Dimension dim;
    if(QString::compare(dimension, "time", Qt::CaseInsensitive) == 0) {
        dim = Time;
    }
    else if(QString::compare(dimension, "x", Qt::CaseInsensitive) == 0
            || QString::compare(dimension, "lon", Qt::CaseInsensitive) == 0)
    {
        dim = Lon;
    }
    else if(QString::compare(dimension, "y", Qt::CaseInsensitive) == 0
            || QString::compare(dimension, "lat", Qt::CaseInsensitive) == 0)
    {
        dim = Lat;
    }
    else if(QString::compare(dimension, "height", Qt::CaseInsensitive) == 0
            || QString::compare(dimension, "z", Qt::CaseInsensitive) == 0)
    {
        dim = Height;
    }
    else {
        BadDimensionString exception;
        exception.setString(dimension);
        throw exception;
    }
    
    return dim;
}

QString dimensionToString(Dimension dimension)
{
    switch(dimension) {
        case Time:
            return QString("Time");
        case Lon:
            return QString("Lon");
        case Lat:
            return QString("Lat");
        case Height:
            return QString("Height");
    }
}

QVariant variantFromString(Dimension dimension, const QString& string)
{
    QVariant variant;
    if(dimension == Time) {
        variant = QDateTime::fromString(string, Qt::ISODate);
    }
    else if(dimension == Lon || dimension == Lat || dimension == Height) {
        variant = string.toDouble();
    }
    
    return variant;
}

QString variantToString(Dimension dimension, const QVariant& variant)
{
    if(dimension == Time) {
        return variant.toDateTime().toString(Qt::ISODate);
    }
    else if(dimension == Lon
            || dimension == Lat
            || dimension == Height)
    {
        QString result;
        return result.setNum(variant.toDouble());
    }
}


