//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QXmlStreamAttributes>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>

// Project
#include "Coverage.h"
#include "CoordinateAxis.h"
#include "Constant.h"
#include "Dimension.h"
#include "global.h"
#include "ParseTime.h"

// Self
#include "CoveragesParser.h"

CoveragesParser::CoveragesParser()
{
}

CoveragesParser::~CoveragesParser()
{
}
    
void CoveragesParser::read()
{
    m_coverages.clear();

    while(!atEnd()) {
        readNext();

        if(isStartElement()) {
            if(name() == "Coverages")
                readCoverages();
        }
    }
}

void CoveragesParser::readCoverages()
{
    Q_ASSERT(isStartElement()
             && name() == "Coverages");
    
    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement()) {
            if(name() == "Coverage") {
                m_coverages.append(readCoverage());
            }
            else {
                readUnknownElement();
            }
        }
    }
}

Coverage CoveragesParser::readCoverage()
{
    Q_ASSERT(isStartElement()
             && name() == "Coverage");
    
    Coverage coverage;
    QList<Constant> constants;
    QList<CoordinateAxis> axes;
    
    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement()) {
            if(name() == "CoverageId") {
                coverage.setCoverageId(readCharacters());
            }
            else if(name() == "Constant") {
                constants.append(readConstant());
            }
            else if(name() == "CoordinateAxis") {
                axes.append(readCoordinateAxis());
            }
            else {
                readUnknownElement();
            }
        }
    }
    
    coverage.setConstants(constants);
    coverage.setCoordinateAxes(axes);
    return coverage;
}

Constant CoveragesParser::readConstant()
{
    Q_ASSERT(isStartElement()
             && name() == "Constant");
    
    Dimension dimension = Lon;
    QVariant value;
    
    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement()) {
            if(name() == "Dimension") {
                dimension = dimensionFromString(readCharacters());
            }
            else if(name() == "Value") {
                if(dimension == Time) {
                    value = parseTime(readCharacters());
                }
                else {
                    value = readCharacters().toDouble();
                }
            }
            else {
                readUnknownElement();
            }
        }
    }
    
    Constant constant(dimension);
    try {
        constant.setValue(value);
    }
    catch (BadDimensionTypeException e) {
        qDebug() << "ERROR, BadDimensionTypeException:" << e.what();
    }
    return constant;
}

CoordinateAxis CoveragesParser::readCoordinateAxis()
{
    Q_ASSERT(isStartElement()
             && name() == "CoordinateAxis");
    
    Dimension dimension = Lon;
    QVariant lowerLimit, upperLimit;
    int valueCount = 0;
    
    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement()) {
            if(name() == "Dimension") {
                dimension = dimensionFromString(readCharacters());
            }
            else if(name() == "LowerLimit") {
                if(dimension == Time) {
                    lowerLimit = parseTime(readCharacters());
                }
                else {
                    lowerLimit = readCharacters().toDouble();
                }
            }
            else if(name() == "UpperLimit") {
                if(dimension == Time) {
                    upperLimit = parseTime(readCharacters());
                }
                else {
                    upperLimit = readCharacters().toDouble();
                }
            }
            else if(name() == "ValueCount") {
                valueCount = readCharacters().toInt();
            }
            else {
                readUnknownElement();
            }
        }
    }
    
    CoordinateAxis axis(dimension);
    try {
        axis.setLowerLimit(lowerLimit);
    }
    catch (BadDimensionTypeException e) {
        qDebug() << "ERROR, BadDimensionTypeException:" << e.what();
    }
    
    try {
        axis.setUpperLimit(upperLimit);
    }
    catch (BadDimensionTypeException e) {
        qDebug() << "ERROR, BadDimensionTypeException:" << e.what();
    }
    return axis;
}


void CoveragesParser::readUnknownElement()
{
    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement())
            readUnknownElement();
    }
}

QString CoveragesParser::readCharacters()
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

QList<Coverage> CoveragesParser::coverages() const
{
    return m_coverages;
}
