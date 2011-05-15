//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DIMENSIONSUBSET_P_H
#define DIMENSIONSUBSET_P_H

// Qt
#include <QtCore/QAtomicInt>

// Self
#include "DimensionSubset.h"

class DimensionSubsetPrivate {
public:
    DimensionSubsetPrivate();
    
    DimensionSubsetPrivate& operator=(const DimensionSubsetPrivate &other);
    bool operator==(const DimensionSubsetPrivate &other);
    
    DimensionSubset::Dimension m_dimension;
    QAtomicInt ref;
};

#endif
