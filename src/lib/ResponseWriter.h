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
    void writeRangeSet(GridCoverage *gridCoverage);
    void writeDataBlock(GridCoverage *gridCoverage);
    void writeTupleList(GridCoverage *gridCoverage);
    
    void writeRangeType(GridCoverage *gridCoverage);
    void writeField(GridCoverage *gridCoverage);
    void writeQuantity(GridCoverage *gridCoverage);
};

#endif
