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

    int layerCount(int lon, int lat) const;
    long start(int lon, int lat) const;

    int maximumLayerCount() const;

    /**
     * Set the dimension of the height.
     * The real world height of a point is
     * layerNumber * heightDimension
     */
    void setHeightDimension(double heightDimension);
    /**
     * Get the dimension of the height.
     * The real world height of a point is
     * layerNumber * heightDimension
     */
    double heightDimension() const;

    MapGeometry& operator=( const MapGeometry &other );

    void detach();

private:
    MapGeometryPrivate *d;
};

#endif
