//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DIMENSIONTRIM_H
#define DIMENSIONTRIM_H

// Project
#include "DimensionSubset.h"

class DimensionTrimPrivate;
class QVariant;

class DimensionTrim : public DimensionSubset
{
public:
    DimensionTrim();
    DimensionTrim(const DimensionTrim &other);

    virtual ~DimensionTrim();

    void setTrimLow(QVariant trimLow);
    QVariant trimLow() const;

    void setTrimHigh(QVariant trimHigh);
    QVariant trimHigh() const;

    DimensionTrim& operator=(const DimensionTrim &other);
    bool operator==(const DimensionTrim &other) const;

    void detach();

private:
    DimensionTrimPrivate *p() const;
};

#endif
