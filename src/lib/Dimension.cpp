//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

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
