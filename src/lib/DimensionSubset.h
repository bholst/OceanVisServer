//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DIMENSIONSUBSET_H
#define DIMENSIONSUBSET_H

// Project
#include "BadDimensionString.h"
#include "BadDimensionTypeException.h"

class QVariant;
class DimensionSubsetPrivate;

class DimensionSubset
{
public:
    DimensionSubset(Dimension dimension);
    /** 
     * Create a new Dimension subset and parse the dimension from the given
     * string.
     * Possible strings:
     * - Time: "time"
     * - Lon: "lon", "x"
     * - Lat: "lat", "y"
     * - Height: "height", "z"
     */
    DimensionSubset(QString dimension) throw (BadDimensionString);
    DimensionSubset(const DimensionSubset &other);
    DimensionSubset(DimensionSubsetPrivate *d);

    virtual ~DimensionSubset();

    Dimension dimension() const;

    DimensionSubset& operator=(const DimensionSubset &other);
    bool operator==(const DimensionSubset &other) const;

    void detach();

protected:
    void ensureDimensionType(const QVariant& value) const throw (BadDimensionTypeException);

    DimensionSubsetPrivate *d;

private:
    DimensionSubsetPrivate *p() const;
};

#endif
