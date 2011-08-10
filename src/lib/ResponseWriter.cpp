//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QVariant>

// Project
#include "GridCoverage.h"
#include "CoordinateAxis.h"
#include "Constant.h"
#include "DataLayer.h"
#include "Dimension.h"
#include "ColorMap.h"

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

void ResponseWriter::write(BadSlicePosition badSlicePosition)
{
    setAutoFormatting(true);
    writeStartDocument();
    
    writeStartElement("BadSlicePosition");
    
    writeStartElement("description");
    writeCharacters(badSlicePosition.what());
    writeEndElement();
    
    writeStartElement("dimension");
    writeCharacters(dimensionToString(badSlicePosition.dimension()));
    writeEndElement();
    
    writeStartElement("givenValue");
    writeCharacters(variantToString(badSlicePosition.dimension(), badSlicePosition.givenValue()));
    writeEndElement();
    
    writeStartElement("lowerLimit");
    writeCharacters(variantToString(badSlicePosition.dimension(), badSlicePosition.lowerLimit()));
    writeEndElement();
    
    writeStartElement("upperLimit");
    writeCharacters(variantToString(badSlicePosition.dimension(), badSlicePosition.upperLimit()));
    writeEndElement();
    
    writeEndElement();
    
    writeEndDocument();
}

void ResponseWriter::writeWrongOvpVersion()
{
    setAutoFormatting(true);
    writeStartDocument();
    
    writeStartElement("WrongVersion");
    writeStartElement("description");
    writeCharacters("The server does not support the protocol version you requested");
    writeEndElement();
    writeStartElement("supportedVersion");
    writeCharacters("1.0.0");
    writeEndElement();
    writeEndElement();
    writeEndDocument();
}

void ResponseWriter::writeCoverages(const QHash<QString,DataLayer *>& layers)
{
    setAutoFormatting(true);
    writeStartDocument();
    
    writeStartElement("Coverages");
    writeServiceIdentification();
    
    for(QHash<QString,DataLayer *>::const_iterator it = layers.constBegin();
        it != layers.constEnd();
        ++it)
    {
        writeCoverage(it.key(), it.value());
    }
    
    writeEndElement();
    
    writeEndDocument();
}

void ResponseWriter::writeServiceIdentification()
{
    writeStartElement("ServiceIdentification");
    
    writeStartElement("Title");
    writeCharacters("Ocean visualization server");
    writeEndElement();
    
    writeStartElement("Abstract");
    writeCharacters("Server developed as a bachelors project at the university of Kiel.");
    writeEndElement();
    
    writeStartElement("ServiceType");
    writeCharacters("OVP");
    writeEndElement();
    
    writeStartElement("ServiceTypeVersion");
    writeCharacters("1.0.0");
    writeEndElement();
    
    writeEndElement();
}

void ResponseWriter::writeCoverage(const QString& name, DataLayer *layer)
{
    writeStartElement("Coverage");
    
    writeStartElement("CoverageId");
    writeCharacters(name);
    writeEndElement();
    
    writeStartElement("minValue");
    writeCharacters(QString::number(layer->minValue()));
    writeEndElement();
    
    writeStartElement("maxValue");
    writeCharacters(QString::number(layer->maxValue()));
    writeEndElement();
    
    layer->defaultColorMap().writeColorMap(this);
    
    QList<CoordinateAxis> coordinateAxes = layer->coordinateAxes();
    foreach(CoordinateAxis axis, coordinateAxes) {
        writeCoordinateAxis(axis);
    }
    
    QList<Constant> consts = layer->constants();
    foreach(Constant constant, consts) {
        writeConstant(constant);
    }
    
    writeEndElement();
}

void ResponseWriter::writeConstant(const Constant& constant)
{
    writeStartElement("Constant");
    
    writeStartElement("Dimension");
    writeCharacters(dimensionToString(constant.dimension()));
    writeEndElement();
    
    writeStartElement("Value");
    writeCharacters(variantToString(constant.dimension(), constant.value()));
    writeEndElement();
    
    writeEndElement();
    writeEndElement();
}

void ResponseWriter::writeCoordinateAxis(const CoordinateAxis& axis)
{
    writeStartElement("CoordinateAxis");
    
    writeStartElement("Dimension");
    writeCharacters(dimensionToString(axis.dimension()));
    writeEndElement();
    
    writeStartElement("LowerLimit");
    writeCharacters(variantToString(axis.dimension(), axis.lowerLimit()));
    writeEndElement();
    
    writeStartElement("UpperLimit");
    writeCharacters(variantToString(axis.dimension(), axis.upperLimit()));
    writeEndElement();
    
    writeStartElement("ValueCount");
    writeCharacters(QString::number(axis.valueCount()));
    writeEndElement();
    
    writeEndElement();
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
            lowerCorner << QString::number(axis.lowerLimit().toDateTime().toTime_t());
        }
        else {
            lowerCorner << QString::number(axis.lowerLimit().toDouble());
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
            upperCorner << QString::number(axis.upperLimit().toDateTime().toTime_t());
        }
        else {
            upperCorner << QString::number(axis.upperLimit().toDouble());
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

