//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <cstdio>

// Qt
#include <QtCore/QDebug>
#include <QtCore/QAtomicInt>

// Self
#include "MapGeometry.h"

class MapGeometryPrivate {
public:
    MapGeometryPrivate()
        : m_layerCounts(0),
          m_starts(0),
          m_width(0),
          m_height(0),
          m_totalSize(-1),
          ref( 1 )
    {
    }

    ~MapGeometryPrivate()
    {
        delete m_layerCounts;
        delete m_starts;
    }
    
    /*
    * return this instead of &other
    */
    MapGeometryPrivate& operator=(const MapGeometryPrivate &other)
    {
        m_width = other.m_width;
        m_height = other.m_height;
        m_totalSize = other.m_totalSize;
        delete m_layerCounts;
        m_layerCounts = new int[m_width * m_height];
        memcpy(m_layerCounts, other.m_layerCounts, m_width * m_height * sizeof(int));
        delete m_starts;
        m_starts = new long[m_width * m_height];
        memcpy(m_starts, other.m_starts, m_width * m_height * sizeof(long));
        ref = other.ref;
        return *this;
    }

    int *m_layerCounts;
    long *m_starts;
    int m_width;
    int m_height;
    int m_totalSize;
    QAtomicInt ref;
};

MapGeometry::MapGeometry()
    : d(new MapGeometryPrivate)
{
}

MapGeometry::MapGeometry(const MapGeometry &other)
    : d(other.d)
{
    d->ref.ref();
}

MapGeometry::~MapGeometry()
{
    if (!d->ref.deref())
        delete d;
}

void MapGeometry::detach()
{
    qAtomicDetach( d );
}

void MapGeometry::setWidth(int width)
{
    detach();
    d->m_width = width;
}

int MapGeometry::width() const
{
    return d->m_width;
}

void MapGeometry::setHeight(int height)
{
    detach();
    d->m_height = height;
}

int MapGeometry::height() const
{
    return d->m_height;
}

void MapGeometry::setLayerCounts(int *layerCounts)
{
    detach();
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
    qAtomicAssign(d, other.d);
    return *this;
}
