//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QXmlStreamAttributes>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>

// Project
#include "DimensionSlice.h"
#include "DimensionTrim.h"
#include "RequestBase.h"
#include "GetCoverage.h"
#include "global.h"

// Self
#include "RequestParser.h"

RequestParser::RequestParser()
    : m_request(0)
{
}

RequestParser::~RequestParser()
{
    delete m_request;
}
    
void RequestParser::read()
{
    delete m_request;
    while(!atEnd()) {
        readNext();

        if(isStartElement()) {
            if(namespaceUri() == "wcs"
               && name() == "GetCoverage")
                readGetCoverage();
        }
    }
}

void RequestParser::readGetCoverage()
{
    Q_ASSERT(isStartElement()
             && namespaceUri() == "wcs"
             && name() == "GetCoverage");
    
    GetCoverage *getCoverage = new GetCoverage();
    m_request = getCoverage;
    QXmlStreamAttributes att = attributes();
    m_request->setVersion(att.value("version").toString());
    QList<DimensionSubset> subsets;
    
    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement()) {
            if(namespaceUri() == "wcs"
               && name() == "CoverageId")
            {
                getCoverage->setCoverageId(readCharacters());
            }
            else if (namespaceUri() == "wcs"
                     && name() == "DimensionTrim")
            {
                subsets.append(readDimensionTrim());
            }
            else if (namespaceUri() == "wcs"
                     && name() == "DimensionSlice")
            {
                subsets.append(readDimensionSlice());
            }
            else {
                readUnknownElement();
            }
        }
    }
    
    getCoverage->setDimensionSubsets(subsets);
}

DimensionTrim RequestParser::readDimensionTrim()
{
    Q_ASSERT(isStartElement()
             && namespaceUri() == "wcs"
             && name() == "DimensionTrim");
    
    Dimension dimension = Time;
    QString trimLowString;
    QString trimHighString;

    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement()) {
            if(namespaceUri() == "wcs"
               && name() == "Dimension")
            {
                QString dim = readCharacters();
                if(dim == "time") {
                    dimension = Time;
                }
                else if(dim == "x" || dim == "lon") {
                    dimension = Lon;
                }
                else if(dim == "y" || dim == "lat") {
                    dimension = Lat;
                }
                else if(dim == "height" || dim == "z") {
                    dimension = Height;
                }
            }
            else if (namespaceUri() == "wcs"
                     && name() == "TrimLow")
            {
                trimLowString = readCharacters();
            }
            else if (namespaceUri() == "wcs"
                     && name() == "TrimHigh")
            {
                trimHighString = readCharacters();
            }
            else {
                readUnknownElement();
            }
        }
    }
    
    DimensionTrim trim(dimension);
    if(dimension == Time) {
        QDateTime trimLow;
        QDateTime trimHigh;
        trimLow.setTime_t(trimLowString.toUInt());
        trim.setTrimLow(trimLow);
        trimHigh.setTime_t(trimHighString.toUInt());
        trim.setTrimHigh(trimHigh);
    }
    else {
        trim.setTrimLow(trimLowString.toDouble());
        trim.setTrimHigh(trimHighString.toDouble());
    }
    
    return trim;
}

DimensionSlice RequestParser::readDimensionSlice()
{
    Q_ASSERT(isStartElement()
             && namespaceUri() == "wcs"
             && name() == "DimensionSlice");

    QString slicePointString;
    QString dimension;

    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement()) {
            if(namespaceUri() == "wcs"
               && name() == "Dimension")
            {
                QString dimension = readCharacters();
            }
            else if (namespaceUri() == "wcs"
                     && name() == "SlicePoint")
            {
                slicePointString = readCharacters();
            }
            else {
                readUnknownElement();
            }
        }
    }
    
    try {
        DimensionSlice slice(dimension);
        
        if(slice.dimension() == Time) {
            QDateTime slicePoint;
            slicePoint.setTime_t(slicePointString.toUInt());
            slice.setSlicePoint(slicePoint);
        }
        else {
            slice.setSlicePoint(slicePointString.toDouble());
        }
        
        return slice;
    } catch (BadDimensionString e) {
        qDebug() << e.what();
        return DimensionSlice(Time);
    }
}

void RequestParser::readUnknownElement()
{
    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement())
            readUnknownElement();
    }
}

QString RequestParser::readCharacters()
{
    Q_ASSERT(isStartElement());

    QString string;

    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement()) {
            readUnknownElement();
        }

        if(isCharacters()) {
            string = text().toString();
        }
    }

    return string;
}
