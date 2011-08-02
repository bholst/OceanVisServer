//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDateTime>

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
    if(dimension == "time") {
        dim = Time;
    }
    else if(dimension == "x" || dimension == "lon") {
        dim = Lon;
    }
    else if(dimension == "y" || dimension == "lat") {
        dim = Lat;
    }
    else if(dimension == "height" || dimension == "z") {
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


