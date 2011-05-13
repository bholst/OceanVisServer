//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <cstdio>

// Qt
#include <QtCore/QDebug>

// Self
#include "MapGeometry.h"

class MapGeometryPrivate {
public:
    MapGeometryPrivate()
        : m_layerCounts(0),
          m_starts(0),
          m_width(0),
          m_height(0),
          m_totalSize(-1)
    {
    }

    ~MapGeometryPrivate()
    {
        delete m_layerCounts;
        delete m_starts;
    }

    int *m_layerCounts;
    long *m_starts;
    int m_width;
    int m_height;
    int m_totalSize;
};

MapGeometry::MapGeometry()
    : d(new MapGeometryPrivate)
{
}

MapGeometry::MapGeometry(const MapGeometry &other)
    : d(new MapGeometryPrivate(*other.d))
{
}

MapGeometry::~MapGeometry()
{
    delete d;
}

void MapGeometry::setWidth(int width)
{
    d->m_width = width;
}

int MapGeometry::width() const
{
    return d->m_width;
}

void MapGeometry::setHeight(int height)
{
    d->m_height = height;
}

int MapGeometry::height() const
{
    return d->m_height;
}

void MapGeometry::setLayerCounts(int *layerCounts)
{
    d->m_layerCounts = new int[d->m_width * d->m_height];
    d->m_starts = new long[d->m_width * d->m_height];
    long currentStart = 0;
    for(int i = 0; i < d->m_width * d->m_height; ++i) {
        d->m_layerCounts[i] = layerCounts[i];
        d->m_starts[i] = currentStart;
        currentStart += d->m_layerCounts[i];
    }

    d->m_totalSize = currentStart;
}

int MapGeometry::layerCount(int x, int y) const
{
    if(x >= d->m_width || y >= d->m_height) {
        return -1;
    }

    return d->m_layerCounts[x + y * d->m_width];
}

long MapGeometry::start(int x, int y) const
{
    if(x >= d->m_width || y >= d->m_height) {
        return -1;
    }

    return d->m_starts[x + y * d->m_width];
}

long MapGeometry::totalSize() const
{
    return d->m_totalSize;
}

MapGeometry& MapGeometry::operator=( const MapGeometry &other )
{
    *d = *other.d;
    return *this;
}
