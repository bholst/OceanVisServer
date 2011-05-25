//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DIMENSIONSLICE_H
#define DIMENSIONSLICE_H

// Project
#include "DimensionSubset.h"
#include "Dimension.h"
#include "BadDimensionTypeException.h"

class DimensionSlicePrivate;
class QVariant;

class DimensionSlice : public DimensionSubset
{
public:
    DimensionSlice(Dimension dimension);
    DimensionSlice(const DimensionSlice &other);

    virtual ~DimensionSlice();

    void setSlicePoint(QVariant slicePoint) throw (BadDimensionTypeException);
    QVariant slicePoint() const;

    DimensionSlice& operator=(const DimensionSlice &other);
    bool operator==(const DimensionSlice &other) const;

    void detach();

private:
    DimensionSlicePrivate *p() const;
};

#endif
