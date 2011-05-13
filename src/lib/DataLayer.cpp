//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Project
#include "MapGeometry.h"

// Self
#include "DataLayer.h"

DataLayer::DataLayer()
{
}

DataLayer::~DataLayer()
{
}
    
MapGeometry DataLayer::geometry() const
{
    return m_geometry;
}

void DataLayer::setGeometry(const MapGeometry &mapGeometry)
{
    m_geometry = mapGeometry;
}
