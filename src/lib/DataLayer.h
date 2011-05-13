//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DATALAYER_H
#define DATALAYER_H

class MapGeometry;

class DataLayer {
public:
    DataLayer();
    virtual ~DataLayer();
    
    MapGeometry *geometry() const;
    void setGeometry(MapGeometry *mapGeometry);

private:
    MapGeometry *m_geometry;
};

#endif
