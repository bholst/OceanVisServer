//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef MAPGEOMETRY_H
#define MAPGEOMETRY_H

class MapGeometry {
public:
    MapGeometry();
    virtual ~MapGeometry();

    void setWidth(int width);
    int width() const;

    void setHeight(int height);
    int height() const;

    long totalSize() const;

    void setLayerCounts(int *layerCounts);

    int layerCount(int x, int y) const;
    long start(int x, int y) const;

private:
    int *m_layerCounts;
    long *m_starts;

    int m_width;
    int m_height;
    int m_totalSize;
};

#endif
