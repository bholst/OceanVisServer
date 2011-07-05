//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

class BoundingBox {
public:
    BoundingBox();
    ~BoundingBox();
    
    void setLonMin(double lonMin);
    double lonMin() const;
    void setLonMax(double lonMax);
    double lonMax() const;
    void setLatMin(double latMin);
    double latMin() const;
    void setLatMax(double latMax);
    double latMax() const;
    
private:
    double m_lonMin;
    double m_lonMax;
    double m_latMin;
    double m_latMax;
};

#endif
