//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DIMENSION_H
#define DIMENSION_H

// Project
#include "global.h"
#include "BadDimensionTypeException.h"
#include "BadDimensionString.h"

class QVariant;

void checkDimensionType(Dimension dimension, const QVariant& value) throw (BadDimensionTypeException);

Dimension dimensionFromString(QString dimension) throw (BadDimensionString);

#endif
