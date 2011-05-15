//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DIMENSIONSUBSET_H
#define DIMENSIONSUBSET_H

class DimensionSubsetPrivate;

class DimensionSubset
{
public:
    enum Dimension {
        Time,
        Lon,
        Lat,
        Height
    };
    
    DimensionSubset();
    DimensionSubset(const DimensionSubset &other);
    DimensionSubset(DimensionSubsetPrivate *d);
    
    virtual ~DimensionSubset();
    
    void setDimension(Dimension dimension);
    Dimension dimension() const;
    
    DimensionSubset& operator=(const DimensionSubset &other);
    bool operator==(const DimensionSubset &other) const;
    
    void detach();
    
protected:
    DimensionSubsetPrivate *d;
    
private:
    DimensionSubsetPrivate *p() const;
};

#endif
