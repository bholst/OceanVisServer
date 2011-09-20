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
#include "ParseTime.h"

GetCoverage::GetCoverage(RequestBase::RequestType type)
    : RequestBase(type),
      m_cutMode(DataLayer::Contains)
{
    if(type == RequestBase::OVP) {
        m_format = "image/png";
    }
    else {
        m_format = "text/xml";
    }
        
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
    qDeleteAll(m_dimensionSubsets);
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

void GetCoverage::setCutMode(DataLayer::CutMode mode)
{
    m_cutMode = mode;
}

DataLayer::CutMode GetCoverage::cutMode() const
{
    return m_cutMode;
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

GetCoverage *GetCoverage::fromRequestString(QString request, RequestBase::RequestType type)
{
    QStringList args = request.split('&');
    GetCoverage *getCoverage = new GetCoverage(type);
    
    foreach(QString arg, args) {
        QStringList argParts = arg.split('=');
        if(argParts.size() != 2) {
            qDebug() << "Strange arguments.";
            continue;
        }
        
        else if(QString::compare(argParts[0], "VERSION", Qt::CaseInsensitive) == 0) {
            getCoverage->setVersion(argParts[1]);
        }
        else if(QString::compare(argParts[0], "COVERAGEID", Qt::CaseInsensitive) == 0) {
            getCoverage->setCoverageId(argParts[1]);
        }
        else if(QString::compare(argParts[0], "FORMAT", Qt::CaseInsensitive) == 0) {
            if(type == RequestBase::OVP
               && !argParts[1].startsWith("image/"))
            {
                qDebug() << "OVP requests only support image formats, defaulting to" << getCoverage->format();
            }
            else {
                getCoverage->setFormat(argParts[1]);
            }
        }
        else if(QString::compare(argParts[0], "CUTMODE", Qt::CaseInsensitive) == 0) {
            if(QString::compare(argParts[1], "contains", Qt::CaseInsensitive) == 0) {
                getCoverage->setCutMode(DataLayer::Contains);
            }
            else if(QString::compare(argParts[1], "overlaps", Qt::CaseInsensitive) == 0) {
                getCoverage->setCutMode(DataLayer::Overlaps);
            }
        }
        else if(QString::compare(argParts[0], "SIZE", Qt::CaseInsensitive) == 0) {
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
        else if(QString::compare(argParts[0], "SUBSET", Qt::CaseInsensitive) == 0
            || QString::compare(argParts[0], "TRIM", Qt::CaseInsensitive) == 0
            || QString::compare(argParts[0], "SLICE", Qt::CaseInsensitive) == 0)
        {
            qDebug() << "Parsing subset.";
            QString arg = argParts[1];
            QRegExp trimExp("([a-zA-Z]{1,})(|,http://www.opengis.net/def/crs/EPSG/0/4326)\\(([-+]?[0-9]{0,}\\.?[0-9]{0,}),([-+]?[0-9]{0,}\\.?[0-9]*)\\)");
            trimExp.indexIn(arg);
            if(trimExp.pos() > -1) {
                qDebug() << "Found trim.";
                QString axis = trimExp.cap(1);
                QString min = trimExp.cap(3);
                QString max = trimExp.cap(4);
                
                qDebug() << "Axis:" << axis;
                qDebug() << "Min:" << min;
                qDebug() << "Max:" << max;
                
                try {
                    DimensionTrim *trim = new DimensionTrim(axis);
                    
                    if(trim->dimension() == Time) {
                        trim->setTrimLow(parseTime(min));
                        trim->setTrimHigh(parseTime(max));
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
                QRegExp sliceExp("([a-zA-Z]{1,})(|,http://www.opengis.net/def/crs/EPSG/0/4326)\\((\\S{1,})\\)");
                sliceExp.indexIn(arg);
                if(sliceExp.pos() > -1) {
                    qDebug() << "Fount slice";
                    QString axis = sliceExp.cap(1);
                    QString slicePointString = sliceExp.cap(3);
                    
                    qDebug() << "Axis:" << axis;
                    qDebug() << "Slice point:" << slicePointString;
                    
                    try {
                        DimensionSlice *slice = new DimensionSlice(axis);
                        
                        if(slice->dimension() == Time) {
                            slice->setSlicePoint(QVariant(parseTime(slicePointString)));
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

QString GetCoverage::toRequestString(const QString& url) const
{
    QString request = "?SERVICE=%1"
                      "&REQUEST=GetCoverage"
                      "&VERSION=%2"
                      "&COVERAGEID=%3"
                      "&FORMAT=%4"
                      "%5" // SIZES
                      "%6" // SUBSETS
                      "&CUTMODE=%7";
    
    QString service;
    if(requestType() == RequestBase::OVP) {
        service = "ovp";
    }
    else if(requestType() == RequestBase::WCS) {
        service = "wcs";
    }
    request = request.arg(service);
    
    request = request.arg(version());
    request = request.arg(coverageId());
    request = request.arg(format());
    
    QString sizes;
    for(QMap<Dimension, int>::const_iterator it = m_sizes.constBegin();
        it != m_sizes.constEnd();
        ++it)
    {
        QString size("&SIZE=%1(%2)");
        sizes += size.arg(dimensionToString(it.key())).arg(it.value());
    }
    request = request.arg(sizes);
    
    QString subsets;
    foreach(DimensionSubset *subset, dimensionSubsets()) {
        DimensionTrim *trim = dynamic_cast<DimensionTrim *>(subset);
        DimensionSlice *slice = dynamic_cast<DimensionSlice *>(subset);
        
        if(trim) {
            QString trimStr = "&SUBSET=%1(%2,%3)";
            subsets += trimStr.arg(dimensionToString(trim->dimension()).toLower())
                              .arg(variantToString(trim->dimension(), trim->trimLow()))
                              .arg(variantToString(trim->dimension(), trim->trimHigh()));
        }
        else if(slice) {
            QString sliceStr = "&SUBSET=%1(%2)";
            subsets += sliceStr.arg(dimensionToString(slice->dimension()).toLower())
                               .arg(variantToString(slice->dimension(), slice->slicePoint()));
        }
    }
    request = request.arg(subsets);
    
    if(cutMode() == DataLayer::Contains) {
        request = request.arg("contains");
    }
    else {
        request = request.arg("overlaps");
    }
    
    return url + request;
}

