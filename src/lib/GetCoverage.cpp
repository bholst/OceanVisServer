//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <QtCore/QDateTime>

// Self
#include "RequestParser.h"
#include "GetCoverage.h"
#include "DimensionSlice.h"
#include "DimensionTrim.h"

GetCoverage::GetCoverage()
    : RequestBase(RequestBase::WCS),
      m_format("text/xml")
{
}

GetCoverage::~GetCoverage()
{
    qDeleteAll(m_dimensionSubsets);
}

QString GetCoverage::request() const
{
    return QString("GetCoverage");
}
    
QList<DimensionSubset*> GetCoverage::dimensionSubsets() const
{
    return m_dimensionSubsets;
}

void GetCoverage::setDimensionSubsets(const QList<DimensionSubset*> &dimensionSubsets)
{
    m_dimensionSubsets = dimensionSubsets;
}

void GetCoverage::addDimensionSubset(DimensionSubset* dimensionSubset)
{
    m_dimensionSubsets.append(dimensionSubset);
}

QString GetCoverage::coverageId() const
{
    return m_coverageId;
}

void GetCoverage::setCoverageId(const QString& coverageId)
{
    m_coverageId = coverageId;
}

void GetCoverage::setFormat(const QString& format)
{
    m_format = format;
}

QString GetCoverage::format() const
{
    return m_format;
}

void GetCoverage::setSize(Dimension dimension, int size)
{
    m_sizes.insert(dimension, size);
}
    
int GetCoverage::size(Dimension dimension) const
{
    return m_sizes.value(dimension);
}

QMap<Dimension, int> GetCoverage::sizes() const
{
    return m_sizes;
}

GetCoverage *GetCoverage::fromRequestString(QString request)
{
    QStringList args = request.split('&');
    GetCoverage *getCoverage = new GetCoverage();
    
    foreach(QString arg, args) {
        QStringList argParts = arg.split('=');
        if(argParts.size() != 2) {
            qDebug() << "Strange arguments.";
            continue;
        }
        
        else if(argParts[0] == "VERSION") {
            getCoverage->setVersion(argParts[1]);
        }
        else if(argParts[0] == "COVERAGEID") {
            getCoverage->setCoverageId(argParts[1]);
        }
        else if(argParts[0] == "FORMAT") {
            getCoverage->setFormat(argParts[1]);
        }
        else if(argParts[0] == "SIZE") {
            QRegExp sizeExp("([a-zA-Z]+)\\(([0-9]+)\\)");
            sizeExp.indexIn(argParts[1]);
            if(sizeExp.pos() > -1) {
                try {
                    getCoverage->setSize(dimensionFromString(sizeExp.cap(1)), sizeExp.cap(2).toInt());
                } catch (BadDimensionString e) {
                    qDebug() << e.what();
                    delete getCoverage;
                    return 0;
                }
            }
        }
        else if(argParts[0] == "SUBSET"
            || argParts[0] == "TRIM"
            || argParts[0] == "SLICE")
        {
            qDebug() << "Parsing subset.";
            QString arg = argParts[1];
            QRegExp trimExp("([a-zA-Z]{1,}),http://www.opengis.net/def/crs/EPSG/0/4326\\(([-+]?[0-9]{0,}\\.?[0-9]{0,}),([-+]?[0-9]{0,}\\.?[0-9]*)\\)");
            trimExp.indexIn(arg);
            if(trimExp.pos() > -1) {
                qDebug() << "Found trim.";
                QString axis = trimExp.cap(1);
                QString min = trimExp.cap(2);
                QString max = trimExp.cap(3);
                
                qDebug() << "Axis:" << axis;
                qDebug() << "Min:" << min;
                qDebug() << "Max:" << max;
                
                try {
                    DimensionTrim *trim = new DimensionTrim(axis);
                    
                    if(trim->dimension() == Time) {
                        trim->setTrimLow(RequestParser::parseTime(min));
                        trim->setTrimHigh(RequestParser::parseTime(max));
                    }
                    else {
                        trim->setTrimLow(min.toDouble());
                        trim->setTrimHigh(max.toDouble());
                    }
                    getCoverage->addDimensionSubset(trim);
                } catch (BadDimensionString e) {
                    qDebug() << e.what();
                    delete getCoverage;
                    return 0;
                }
            }
            else {
                QRegExp sliceExp("([a-zA-Z]{1,}),http://www.opengis.net/def/crs/EPSG/0/4326\\((\\S{1,})\\)");
                sliceExp.indexIn(arg);
                if(sliceExp.pos() > -1) {
                    qDebug() << "Fount slice";
                    QString axis = sliceExp.cap(1);
                    QString slicePointString = sliceExp.cap(2);
                    
                    qDebug() << "Axis:" << axis;
                    qDebug() << "Slice point:" << slicePointString;
                    
                    try {
                        DimensionSlice *slice = new DimensionSlice(axis);
                        
                        if(slice->dimension() == Time) {
                            slice->setSlicePoint(QVariant(RequestParser::parseTime(slicePointString)));
                        }
                        else {
                            slice->setSlicePoint(slicePointString.toDouble());
                        }
                        getCoverage->addDimensionSubset(slice);
                    } catch (BadDimensionString e) {
                        qDebug() << e.what();
                        delete getCoverage;
                        return 0;
                    }
                }
            }
        }
    }
    
    return getCoverage;
}

