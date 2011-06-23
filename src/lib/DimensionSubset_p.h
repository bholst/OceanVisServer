//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DIMENSIONSUBSET_P_H
#define DIMENSIONSUBSET_P_H

// Qt
#include <QtCore/QAtomicInt>

// Project
#include "Dimension.h"
#include "BadDimensionString.h"

class QString;

class DimensionSubsetPrivate {
public:
    DimensionSubsetPrivate(Dimension dimension);
    DimensionSubsetPrivate(QString dimension) throw (BadDimensionString);
    DimensionSubsetPrivate(const DimensionSubsetPrivate& other);

    virtual ~DimensionSubsetPrivate();
    
    DimensionSubsetPrivate& operator=(const DimensionSubsetPrivate &other);
    bool operator==(const DimensionSubsetPrivate &other);
    
    Dimension m_dimension;
    QAtomicInt ref;
};

#endif
