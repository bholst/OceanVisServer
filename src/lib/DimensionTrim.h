//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DIMENSIONTRIM_H
#define DIMENSIONTRIM_H

// Project
#include "DimensionSubset.h"
#include "BadDimensionTypeException.h"

class DimensionTrimPrivate;
class QVariant;

class DimensionTrim : public DimensionSubset
{
public:
    DimensionTrim(Dimension dimension);
    /** 
     * Create a new Dimension trim and parse the dimension from the given
     * string.
     * Possible strings:
     * - Time: "time"
     * - Lon: "lon", "x"
     * - Lat: "lat", "y"
     * - Height: "height", "z"
     */
    DimensionTrim(QString dimension) throw (BadDimensionString);
    DimensionTrim(const DimensionTrim &other);

    virtual ~DimensionTrim();

    void setTrimLow(QVariant trimLow) throw(BadDimensionTypeException);
    QVariant trimLow() const;

    void setTrimHigh(QVariant trimHigh) throw(BadDimensionTypeException);
    QVariant trimHigh() const;

    DimensionTrim& operator=(const DimensionTrim &other);
    bool operator==(const DimensionTrim &other) const;

    void detach();

private:
    DimensionTrimPrivate *p() const;
};

#endif
