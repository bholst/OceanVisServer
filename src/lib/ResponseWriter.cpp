//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QStringList>
#include <QtCore/QDateTime>

// Project
#include "GridCoverage.h"
#include "CoordinateAxis.h"

// Self
#include "ResponseWriter.h"

ResponseWriter::ResponseWriter()
{
}

ResponseWriter::~ResponseWriter()
{
}
    
void ResponseWriter::write(GridCoverage *gridCoverage)
{
    setAutoFormatting(true);
    writeStartDocument();
    
    writeNamespace("http://www.opengis.net/gmlcov/1.0", "gmlcov");
    writeStartElement("http://www.opengis.net/gmlcov/1.0", "GridCoverage");
    writeNamespace("http://www.opengis.net/gml/3.2", "gml");
    writeNamespace("http://www.opengis.net/swe/2.0", "swe");
    writeBoundedBy(gridCoverage);
    writeRangeType(gridCoverage);
    writeRangeSet(gridCoverage);
    writeEndElement();
    writeEndDocument();
}

void ResponseWriter::writeBoundedBy(GridCoverage *gridCoverage)
{
    writeStartElement("http://www.opengis.net/gml/3.2", "boundedBy");
    writeEnvelope(gridCoverage);
    writeEndElement();
}
void ResponseWriter::writeEnvelope(GridCoverage *gridCoverage)
{
    writeStartElement("http://www.opengis.net/gml/3.2", "Envelope");
    writeAttribute("srsName", "http://www.opengis.net/def/crs/EPSG/0/4326");
    QStringList axisLabels;
    QStringList uomLabels;
    foreach(CoordinateAxis axis, gridCoverage->coordinateAxes()) {
        if(axis.dimension() == Lon) {
            axisLabels << "Long";
            uomLabels << "deg";
        }
        else if(axis.dimension() == Lat) {
            axisLabels << "Lat";
            uomLabels << "deg";
        }
        else if(axis.dimension() == Time) {
            axisLabels << "Time";
            uomLabels << "sec";
        }
        else if(axis.dimension() == Height) {
            axisLabels << "Height";
            uomLabels << "units";
        }
    }
    
    writeAttribute("axisLabels", axisLabels.join(" "));
    writeAttribute("uomLabels", uomLabels.join(" "));
    writeAttribute("srsDimension", QString::number(gridCoverage->coordinateAxes().size()));
    writeLowerCorner(gridCoverage);
    writeUpperCorner(gridCoverage);
    writeEndElement();
}

void ResponseWriter::writeLowerCorner(GridCoverage *gridCoverage)
{
    writeStartElement("http://www.opengis.net/gml/3.2", "lowerCorner");
    QStringList lowerCorner;
    foreach(CoordinateAxis axis, gridCoverage->coordinateAxes()) {
        if(axis.dimension() == Time) {
            lowerCorner << QString::number(axis.minValue().toDateTime().toTime_t());
        }
        else {
            lowerCorner << QString::number(axis.minValue().toDouble());
        }
    }
    writeCharacters(lowerCorner.join(" "));
    writeEndElement();
}

void ResponseWriter::writeUpperCorner(GridCoverage *gridCoverage)
{
    writeStartElement("http://www.opengis.net/gml/3.2", "lowerCorner");
    QStringList upperCorner;
    foreach(CoordinateAxis axis, gridCoverage->coordinateAxes()) {
        if(axis.dimension() == Time) {
            upperCorner << QString::number(axis.maxValue().toDateTime().toTime_t());
        }
        else {
            upperCorner << QString::number(axis.maxValue().toDouble());
        }
    }
    writeCharacters(upperCorner.join(" "));
    writeEndElement();
}

void ResponseWriter::writeRangeType(GridCoverage *gridCoverage)
{
    writeStartElement("http://www.opengis.net/gmlcov/1.0", "RangeType");
    writeField(gridCoverage);
    writeEndElement();
}

void ResponseWriter::writeField(GridCoverage *gridCoverage)
{
    writeStartElement("http://www.opengis.net/swe/2.0", "field");
    writeAttribute("name", gridCoverage->name());
    writeQuantity(gridCoverage);
    writeEndElement();
}

void ResponseWriter::writeQuantity(GridCoverage *gridCoverage)
{
}
    
void ResponseWriter::writeRangeSet(GridCoverage *gridCoverage)
{
    writeStartElement("http://www.opengis.net/gml/3.2", "rangeSet");
    writeDataBlock(gridCoverage);
    writeEndElement();
}
    
void ResponseWriter::writeDataBlock(GridCoverage *gridCoverage)
{
    writeStartElement("DataBlock");
    writeEmptyElement("rangeParameters");
    writeTupleList(gridCoverage);
    writeEndElement();
}

void ResponseWriter::writeTupleList(GridCoverage *gridCoverage)
{
    writeStartElement("tupleList");
    writeCharacters(gridCoverage->toString());
    writeEndElement();
}

