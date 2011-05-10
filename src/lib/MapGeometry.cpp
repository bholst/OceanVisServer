//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <cstdio>

// Qt
#include <QtCore/QDebug>

// Self
#include "MapGeometry.h"

MapGeometry::MapGeometry()
    : m_layerCounts(0),
      m_starts(0),
      m_width(0),
      m_height(0),
      m_totalSize(-1)
{
}

MapGeometry::~MapGeometry()
{
    delete m_layerCounts;
    delete m_starts;
}

void MapGeometry::setWidth(int width)
{
    m_width = width;
}

int MapGeometry::width() const
{
    return m_width;
}

void MapGeometry::setHeight(int height)
{
    m_height = height;
}

int MapGeometry::height() const
{
    return m_height;
}

void MapGeometry::setLayerCounts(int *layerCounts)
{
    m_layerCounts = new int[m_width * m_height];
    m_starts = new long[m_width * m_height];
    long currentStart = 0;
    for(int i = 0; i < m_width * m_height; ++i) {
        m_layerCounts[i] = layerCounts[i];
        m_starts[i] = currentStart;
        currentStart += m_layerCounts[i];
    }

    m_totalSize = currentStart;
}

int MapGeometry::layerCount(int x, int y) const
{
    if(x >= m_width || y >= m_height) {
        return -1;
    }

    return m_layerCounts[x + y * m_width];
}

long MapGeometry::start(int x, int y) const
{
    if(x >= m_width || y >= m_height) {
        return -1;
    }

    return m_starts[x + y * m_width];
}

long MapGeometry::totalSize() const
{
    return m_totalSize;
}
