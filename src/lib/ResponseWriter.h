//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef RESPONSEWRITER_H
#define RESPONSEWRITER_H

// Qt
#include <QtCore/QXmlStreamWriter>

class GridCoverage;

class ResponseWriter : public QXmlStreamWriter
{
public:
    ResponseWriter();
    ~ResponseWriter();
    
    void write(GridCoverage *gridCoverage);
    
private:
    void writeBoundedBy(GridCoverage *gridCoverage);
    void writeEnvelope(GridCoverage *gridCoverage);
    void writeLowerCorner(GridCoverage *gridCoverage);
    void writeUpperCorner(GridCoverage *gridCoverage);
    
    void writeRangeType(GridCoverage *gridCoverage);
    void writeField(GridCoverage *gridCoverage);
    void writeQuantity(GridCoverage *gridCoverage);
    
    void writeRangeSet(GridCoverage *gridCoverage);
    void writeDataBlock(GridCoverage *gridCoverage);
    void writeTupleList(GridCoverage *gridCoverage);
};

#endif
