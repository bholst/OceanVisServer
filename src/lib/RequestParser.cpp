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
    QList<DimensionSubset*> subsets;
    
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
                DimensionTrim *trim = readDimensionTrim();
                if(trim) {
                    subsets.append(trim);
                }
            }
            else if (namespaceUri() == "wcs"
                     && name() == "DimensionSlice")
            {
                DimensionSlice *slice = readDimensionSlice();
                if(slice) {
                    subsets.append(slice);
                }
            }
            else {
                readUnknownElement();
            }
        }
    }
    
    getCoverage->setDimensionSubsets(subsets);
}

DimensionTrim *RequestParser::readDimensionTrim()
{
    Q_ASSERT(isStartElement()
             && namespaceUri() == "wcs"
             && name() == "DimensionTrim");
    
    QString dimension;
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
                dimension = readCharacters();
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
    
    try {
        DimensionTrim *trim = new DimensionTrim(dimension);
        
        if(trim->dimension() == Time) {
            trim->setTrimLow(parseTime(trimLowString));
            trim->setTrimHigh(parseTime(trimHighString));
        }
        else {
            trim->setTrimLow(trimLowString.toDouble());
            trim->setTrimHigh(trimHighString.toDouble());
        }
        
        return trim;
    } catch (BadDimensionString e) {
        qDebug() << e.what();
        return 0;
    }
}

DimensionSlice *RequestParser::readDimensionSlice()
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
        DimensionSlice *slice = new DimensionSlice(dimension);
        
        if(slice->dimension() == Time) {
            slice->setSlicePoint(RequestParser::parseTime(slicePointString));
        }
        else {
            slice->setSlicePoint(slicePointString.toDouble());
        }
        
        return slice;
    } catch (BadDimensionString e) {
        qDebug() << e.what();
        return 0;
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

QDateTime RequestParser::parseTime(QString timeString)
{
    QDateTime time;
    bool success = false;
    uint seconds = timeString.toUInt(&success);
    if(success) {
        qDebug() << "Found seconds.";
        time.setTime_t(seconds);
    }
    else {
        qDebug() << "ISO time.";
    }
    
    return time;
}
