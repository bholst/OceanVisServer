//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Project
#include "MapGeometry.h"

// Self
#include "DataLayer.h"

DataLayer::DataLayer()
    : m_geometry(0)
{
}

DataLayer::~DataLayer()
{
    delete m_geometry;
}
    
MapGeometry *DataLayer::geometry() const
{
    return m_geometry;
}

void DataLayer::setGeometry(MapGeometry *mapGeometry)
{
    m_geometry = mapGeometry;
}
