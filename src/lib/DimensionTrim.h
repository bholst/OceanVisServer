//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DIMENSIONTRIM_H
#define DIMENSIONTRIM_H

// Project
#include "DimensionSubset.h"

class DimensionTrimPrivate;

class DimensionTrim : public DimensionSubset
{
public:
    DimensionTrim();
    DimensionTrim(const DimensionTrim &other);

    virtual ~DimensionTrim();

    void setTrimLow(double trimLow);
    double trimLow() const;

    void setTrimHigh(double trimHigh);
    double trimHigh() const;

    DimensionTrim& operator=(const DimensionTrim &other);
    bool operator==(const DimensionTrim &other) const;

    void detach();

private:
    DimensionTrimPrivate *p() const;
};

#endif
