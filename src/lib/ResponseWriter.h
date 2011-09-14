//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef RESPONSEWRITER_H
#define RESPONSEWRITER_H

// Qt
#include <QtCore/QXmlStreamWriter>
#include <QtCore/QHash>

class GridCoverage;
class DataLayer;
class QString;
class Constant;
class CoordinateAxis;
class QVariant;

class ResponseWriter : public QXmlStreamWriter
{
public:
    ResponseWriter();
    ~ResponseWriter();
    
    void write(GridCoverage *gridCoverage);

    void write(BadSlicePosition badSlicePosition);
    
    void writeWrongOvpVersion();
    
    void writeCoverages(const QHash<QString,DataLayer *>& layers);
    
private:
    void writeServiceIdentification();
    void writeCoverage(const QString& name, DataLayer *layer);
    void writeConstant(const Constant& constant);
    void writeCoordinateAxis(const CoordinateAxis& axis);
    void writeValues(Dimension dimension, const QList<QVariant>& values);
    
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
