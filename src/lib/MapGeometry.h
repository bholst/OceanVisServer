//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef MAPGEOMETRY_H
#define MAPGEOMETRY_H

class MapGeometryPrivate;

class MapGeometry {
public:
    MapGeometry();
    MapGeometry(const MapGeometry &other);
    virtual ~MapGeometry();

    void setWidth(int width);
    int width() const;

    void setHeight(int height);
    int height() const;

    long totalSize() const;

    void setLayerCounts(int *layerCounts);

    int layerCount(int x, int y) const;
    long start(int x, int y) const;
    
    MapGeometry& operator=( const MapGeometry &other );

private:
    MapGeometryPrivate * const d;
};

#endif
