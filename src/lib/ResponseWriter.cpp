//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Project
#include "GridCoverage.h"

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
    writeRangeType(gridCoverage);
    writeRangeSet(gridCoverage);
    writeEndElement();
    writeEndDocument();
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

