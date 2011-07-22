//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <typeinfo>
#include <cmath>
#include <assert.h>
#include <limits>

// Qt
#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDateTime>
#include <QtCore/QMap>

// Project
#include "MapGeometry.h"
#include "CoordinateAxis.h"
#include "DimensionSubset.h"
#include "DimensionSlice.h"
#include "DimensionTrim.h"
#include "GridCoverage.h"

// Self
#include "DataLayer.h"

class DataLayerPrivate {
public:
    DataLayerPrivate()
        : m_scaleMin(0.0),
          m_scaleMax(0.0),
          m_minValue(0.0),
          m_maxValue(0.0)
    {
    }

    ~DataLayerPrivate(){
//         foreach(QFile *file, m_files) {
//             delete file;
//         }
        foreach(double *vec, m_dataVectors) {
            delete vec;
        }
    }

    inline double value(double *dataVector,
                        int lon,
                        int lat,
                        int height) const;

    QString m_name;
    MapGeometry m_geometry;
    QMap<QDateTime, QString> m_fileNames;
//     QMap<QDateTime, QFile *> m_files;
    QMap<QDateTime, double *> m_dataVectors;
    double m_scaleMin;
    double m_minValue;
    double m_scaleMax;
    double m_maxValue;
};

inline double DataLayerPrivate::value(double *dataVector, int lon, int lat, int height) const
{
    if(lon < 0 || lon >= m_geometry.width()
       || lat < 0 || lat >= m_geometry.height())
    {
        return std::numeric_limits<double>::quiet_NaN();
    }
    int layerCount = m_geometry.layerCount(lon, lat);
    assert(layerCount >= 0);
    if(layerCount > height) {
        return dataVector[m_geometry.start(lon, lat) + height];
    }
    else {
        return std::numeric_limits<double>::quiet_NaN();
    }
}

DataLayer::DataLayer()
    : d(new DataLayerPrivate())
{
}

DataLayer::~DataLayer()
{
    delete d;
}

void DataLayer::setName(const QString& name)
{
    d->m_name = name;
}

QString DataLayer::name() const
{
    return d->m_name;
}

MapGeometry DataLayer::geometry() const
{
    return d->m_geometry;
}

void DataLayer::setGeometry(const MapGeometry &mapGeometry)
{
    d->m_geometry = mapGeometry;
}

void DataLayer::setFileName(const QDateTime& dateTime, const QString& fileName)
{
    QFile *file = new QFile(fileName);
    if(!file->open(QIODevice::ReadOnly)) {
        qDebug() << "ERROR" << file->error() << ": File" << fileName << "cannot be opened.";
    }
    else {
//         d->m_files.insert(dateTime, file);
        d->m_fileNames.insert(dateTime, fileName);
        QDataStream stream(file);
        qint32 magicNumber;
        stream >> magicNumber;
        qDebug() << "The magic number is:" << magicNumber;
        int fileLength;
        stream >> fileLength;
        qDebug() << "The length is:" << fileLength;

        double *dataVector = new double[fileLength];
        bool error = false;
        for(int i = 0; i < fileLength; ++i) {
            if(stream.atEnd()) {
                qDebug() << "ERROR: file too short, missing data.";
                error = true;
            }
            stream >> dataVector[i];
            if(d->m_maxValue < dataVector[i]) {
                d->m_maxValue = dataVector[i];
            }
            else if(d->m_minValue > dataVector[i]) {
                d->m_minValue = dataVector[i];
            }
        }
        if(!stream.atEnd()) {
            qDebug() << "ERROR: file to long, too much data.";
            error = true;
        }

        if(!error) {
            d->m_dataVectors.insert(dateTime, dataVector);
        }
        else {
            delete dataVector;
        }
        
        file->close();
        delete file;
    }
}

QString DataLayer::fileName(const QDateTime& dateTime) const
{
    QMap<QDateTime, QString>::const_iterator fileIterator = d->m_fileNames.find(dateTime);
    if(fileIterator != d->m_fileNames.end() && fileIterator.key() == dateTime) {
        return fileIterator.value();
    }
    return QString();
}


void DataLayer::setScaleMin(double scaleMin)
{
    d->m_scaleMin = scaleMin;
}

double DataLayer::scaleMin() const
{
    return d->m_scaleMin;
}

double DataLayer::minValue() const
{
    if(d->m_scaleMin == d->m_scaleMax) {
        return d->m_minValue;
    }
    return d->m_scaleMin;
}

void DataLayer::setScaleMax(double scaleMax)
{
    d->m_scaleMax = scaleMax;
}

double DataLayer::scaleMax() const
{
    return d->m_scaleMax;
}

double DataLayer::maxValue() const 
{
    if(d->m_scaleMin == d->m_scaleMax) {
        return d->m_maxValue;
    }
    return d->m_scaleMax;
}

GridCoverage *DataLayer::dataSubset(QList<DimensionSubset*>& subsets,
                                    DataLayer::CutMode mode)
{
    if(d->m_dataVectors.empty()) {
        return 0;
    }
    
    DimensionSubset *lonSubset = 0;
    DimensionSubset *latSubset = 0;
    QMap<Dimension,DimensionSlice> dimensionSlices;
    QMap<Dimension,DimensionTrim> dimensionTrims;
    for(int i = 0; i < subsets.size(); ++i) {
        DimensionSubset *subset = subsets[i];
        
        switch(subset->dimension()) {
            case Lon:
                lonSubset = subset;
                break;
            case Lat:
                latSubset = subset;
                break;
        }

        DimensionSlice *slice = dynamic_cast<DimensionSlice*>(subset);
        DimensionTrim *trim = dynamic_cast<DimensionTrim*>(subset);
        if(slice) {
            qDebug() << "Subset number" << i << "is a slice";
            dimensionSlices.insert(slice->dimension(), DimensionSlice(*slice));
        }
        else if(trim) {
            qDebug() << "Subset number" << i << "is a trim";
            dimensionTrims.insert(trim->dimension(), DimensionTrim(*trim));
        }
        else {
            qDebug() << "Subset number" << i << "is nothing.";
        }
    }
    QList<CoordinateAxis> axes;

    int dimensionCount[4] = {0, 0, 0, 0};
    QMap<Dimension,DimensionSlice>::const_iterator timeSliceIt = dimensionSlices.find(Time);
    QMap<Dimension,DimensionTrim>::const_iterator timeTrimIt = dimensionTrims.find(Time);
    QMap<QDateTime,double*>::const_iterator lowTimeTrim;
    QMap<QDateTime,double*>::const_iterator highTimeTrim;
    if(timeSliceIt != dimensionSlices.constEnd()) {
        lowTimeTrim = d->m_dataVectors.lowerBound(timeSliceIt->slicePoint().toDateTime());
        if(lowTimeTrim == d->m_dataVectors.constEnd()) {
            lowTimeTrim--;
        }
        highTimeTrim = lowTimeTrim + 1;
        dimensionCount[0] = 1;
    }
    else if(timeTrimIt != dimensionTrims.constEnd()) {
        CoordinateAxis axis(Time);
        QDateTime trimLow = timeTrimIt->trimLow().toDateTime();
        QDateTime trimHigh = timeTrimIt->trimHigh().toDateTime();

        lowTimeTrim = d->m_dataVectors.lowerBound(trimLow);
        if(mode == Overlaps
           && lowTimeTrim != d->m_dataVectors.constBegin())
        {
            lowTimeTrim--;
        }
        axis.setLowerLimit(lowTimeTrim.key());

        highTimeTrim = d->m_dataVectors.upperBound(trimHigh);
        if(mode == Overlaps
           && highTimeTrim != d->m_dataVectors.constEnd())
        {
            highTimeTrim++;
        }

        for(QMap<QDateTime, double*>::const_iterator it = lowTimeTrim;
            it != highTimeTrim;
            ++it)
        {
            dimensionCount[0]++;
            axis.setUpperLimit(it.key());
        }
        axis.setValueCount(dimensionCount[0]);
        axes.append(axis);
    }
    else {
        lowTimeTrim = d->m_dataVectors.begin();
        highTimeTrim = d->m_dataVectors.end();
        dimensionCount[0] = d->m_dataVectors.size();
        CoordinateAxis axis(Time);
        axis.setLowerLimit(lowTimeTrim.key());
        axis.setUpperLimit((highTimeTrim - 1).key());
        axis.setValueCount(dimensionCount[0]);
        axes.append(axis);
    }

    qDebug() << "Low time trim:" << lowTimeTrim.key().toString();
    if(highTimeTrim != d->m_dataVectors.constEnd()) {
        qDebug() << "High time trim:" << highTimeTrim.key().toString();
    }

    QMap<Dimension,DimensionSlice>::const_iterator lonSliceIt = dimensionSlices.find(Lon);
    QMap<Dimension,DimensionTrim>::const_iterator lonTrimIt = dimensionTrims.find(Lon);
    int lowLonTrim = 0; // The first longitude value which will be in the returned matrix.
    int highLonTrim = 0; // The first longitude value which will not be in the returned matrix.

    calculateLonLimits(lonSubset, &lowLonTrim, &highLonTrim, &axes, mode);
    dimensionCount[1] = highLonTrim - lowLonTrim;

    qDebug() << "Low lon trim:" << lowLonTrim;
    qDebug() << "High lon trim:" << highLonTrim;
    qDebug() << "Number of lon values:" << dimensionCount[1];

    QMap<Dimension,DimensionSlice>::const_iterator latSliceIt = dimensionSlices.find(Lat);
    QMap<Dimension,DimensionTrim>::const_iterator latTrimIt = dimensionTrims.find(Lat);
    int lowLatTrim = 0; // The first latitude value which will be in the returned matrix.
    int highLatTrim = 0; // The first latitude value which will not be in the returned matrix.

    calculateLatLimits(latSubset, &lowLatTrim, &highLatTrim, &axes, mode);
    dimensionCount[2] = highLatTrim - lowLatTrim;

    qDebug() << "Low lat trim:" << lowLatTrim;
    qDebug() << "High lat trim:" << highLatTrim;
    qDebug() << "Number of lat values:" << dimensionCount[2];

    QMap<Dimension,DimensionSlice>::const_iterator heightSliceIt = dimensionSlices.find(Height);
    QMap<Dimension,DimensionTrim>::const_iterator heightTrimIt = dimensionTrims.find(Height);
    int lowHeightTrim = 0; // The first latitude value which will be in the returned matrix.
    int highHeightTrim = 0; // The first latitude value which will not be in the returned matrix.

    if(heightSliceIt != dimensionSlices.end()) {
        int slicePoint = round(
            heightSliceIt->slicePoint().toDouble() / d->m_geometry.heightDimension());
        lowHeightTrim = slicePoint;
        highHeightTrim = slicePoint + 1;
        dimensionCount[3] = 1;
    }
    else if(latTrimIt != dimensionTrims.end()) {
        CoordinateAxis axis(Height);
        if(d->m_geometry.heightDimension() >= 0) {
            lowHeightTrim = std::ceil(
                heightTrimIt->trimLow().toDouble() / d->m_geometry.heightDimension());
            highHeightTrim = std::floor(
                heightTrimIt->trimHigh().toDouble() / d->m_geometry.heightDimension()) + 1;
            axis.setLowerLimit(lowHeightTrim * d->m_geometry.heightDimension());
            axis.setUpperLimit(highHeightTrim * d->m_geometry.heightDimension());
        }
        else {
            lowHeightTrim = std::ceil(
                heightTrimIt->trimHigh().toDouble() / d->m_geometry.heightDimension());
            highHeightTrim = std::floor(
                heightTrimIt->trimLow().toDouble() / d->m_geometry.heightDimension()) + 1;
            axis.setLowerLimit(highHeightTrim * d->m_geometry.heightDimension());
            axis.setUpperLimit(lowHeightTrim * d->m_geometry.heightDimension());
        }
        dimensionCount[3] = highHeightTrim - lowHeightTrim;
        axis.setValueCount(dimensionCount[3]);
        axes.append(axis);
    }
    else {
        lowHeightTrim = 0;
        highHeightTrim = d->m_geometry.maxLayerCount();
        dimensionCount[3] = highHeightTrim;

        CoordinateAxis axis(Height);
        if(d->m_geometry.heightDimension() >= 0) {
            axis.setLowerLimit(0.0);
            axis.setUpperLimit(d->m_geometry.maxLayerCount() * d->m_geometry.heightDimension());
        }
        else {
            axis.setLowerLimit(d->m_geometry.maxLayerCount() * d->m_geometry.heightDimension());
            axis.setUpperLimit(0.0);
        }
        axis.setValueCount(dimensionCount[3]);
        axes.append(axis);
    }

    qDebug() << "Low height trim:" << lowHeightTrim;
    qDebug() << "High height trim:" << highHeightTrim;
    qDebug() << "Number of height values:" << dimensionCount[3];

    // Allocating the memory for the data
    qDebug() << "Allocation space:" << dimensionCount[0]
                                       * dimensionCount[1]
                                       * dimensionCount[2]
                                       * dimensionCount[3];
    double *matrix = new double[dimensionCount[0]
                                * dimensionCount[1]
                                * dimensionCount[2]
                                * dimensionCount[3]];

    // Now write the data into the allocated array.
    double *writePos = matrix;

    for(QMap<QDateTime,double*>::const_iterator timeIt = lowTimeTrim;
        timeIt != highTimeTrim;
        ++timeIt)
    {
        for(int lonIt = lowLonTrim; lonIt != highLonTrim; ++lonIt) {
            for(int latIt = lowLatTrim; latIt != highLatTrim; ++latIt) {
                if(d->m_geometry.heightDimension() >= 0) {
                    for(int heightIt = lowHeightTrim; heightIt != highHeightTrim; ++heightIt) {
                        *writePos = d->value(timeIt.value(), lonIt, latIt, heightIt);
                        writePos++;
                    }
                }
                else {
                    for(int heightIt = highHeightTrim - 1; heightIt >= lowHeightTrim; --heightIt) {
                        *writePos = d->value(timeIt.value(), lonIt, latIt, heightIt);
                        writePos++;
                    }
                }
            }
        }
    }

    GridCoverage *result = new GridCoverage();
    result->setValues(matrix);
    result->setCoordinateAxes(axes);
    result->setMaxValue(maxValue());
    result->setMinValue(minValue());
    result->setName(name());
    return result;
}

void DataLayer::calculateLonLimits(DimensionSubset *subset, 
                                   int *lowLonTrim, 
                                   int *highLonTrim, 
                                   QList<CoordinateAxis> *axes, 
                                   DataLayer::CutMode mode)
{
    if(!subset) {
        *lowLonTrim = 0;
        *highLonTrim = d->m_geometry.width();
        CoordinateAxis axis(Lon);
        axis.setLowerLimit(-180.0);
        axis.setUpperLimit(+180.0);
        axis.setValueCount(*highLonTrim - *lowLonTrim);
        axes->append(axis);
        return;
    }

    DimensionSlice *slice = dynamic_cast<DimensionSlice*>(subset);
    DimensionTrim *trim = dynamic_cast<DimensionTrim*>(subset);
    
    if(slice) {
        int slicePoint = std::floor(
            (slice->slicePoint().toDouble() + 180.0) / 360.0 * (double) d->m_geometry.width());
        *lowLonTrim = slicePoint;
        *highLonTrim = slicePoint + 1;
    }
    else if(trim) {
        double lowLonTrimDouble = (trim->trimLow().toDouble() + 180.0) / 360.0 * (double) d->m_geometry.width();
        double highLonTrimDouble = (trim->trimHigh().toDouble() + 180.0) / 360.0 * (double) d->m_geometry.width();
        if(mode == Contains) {
            *lowLonTrim = std::ceil(lowLonTrimDouble);
            *highLonTrim = std::floor(highLonTrimDouble);
        }
        else {
            *lowLonTrim = std::floor(lowLonTrimDouble);
            *highLonTrim = std::ceil(highLonTrimDouble);
        }
        
        CoordinateAxis axis(Lon);
        axis.setLowerLimit(*lowLonTrim * 360.0 / (double) d->m_geometry.width() - 180.0);
        axis.setUpperLimit(*highLonTrim * 360.0 / (double) d->m_geometry.width() - 180.0);
        axis.setValueCount(*highLonTrim - *lowLonTrim);
        axes->append(axis);
    }
    else {
        qDebug() << "Subset invalid.";
        calculateLonLimits(0, lowLonTrim, highLonTrim, axes, mode);
    }
}

void DataLayer::calculateLatLimits(DimensionSubset *subset,
                        int *lowLatTrim,
                        int *highLatTrim,
                        QList<CoordinateAxis> *axes,
                        DataLayer::CutMode mode)
{
    if(!subset) {
        *lowLatTrim = 0;
        *highLatTrim = d->m_geometry.height();

        CoordinateAxis axis(Lat);
        axis.setLowerLimit(-90.0);
        axis.setUpperLimit(+90.0);
        axis.setValueCount(*highLatTrim - *lowLatTrim);
        axes->append(axis);
        return;
    }
    
    DimensionSlice *slice = dynamic_cast<DimensionSlice*>(subset);
    DimensionTrim *trim = dynamic_cast<DimensionTrim*>(subset);
        
    if(slice) {
        int slicePoint = std::floor(
            (slice->slicePoint().toDouble() + 90.0) / 180.0 * (double) d->m_geometry.height());
        *lowLatTrim = slicePoint;
        *highLatTrim = slicePoint + 1;
    }
    else if(trim) {
        double lowLatTrimDouble = (trim->trimLow().toDouble() + 90.0) / 180.0 * (double) d->m_geometry.height();
        double highLatTrimDouble = (trim->trimHigh().toDouble() + 90.0) / 180.0 * (double) d->m_geometry.height();
        if(mode == Contains) {
            *lowLatTrim = std::ceil(lowLatTrimDouble);
            *highLatTrim = std::floor(highLatTrimDouble);
        }
        else {
            *lowLatTrim = std::floor(lowLatTrimDouble);
            *highLatTrim = std::ceil(highLatTrimDouble);
        }

        CoordinateAxis axis(Lat);
        axis.setLowerLimit(*lowLatTrim * 180.0 / (double) d->m_geometry.height() - 90.0);
        axis.setUpperLimit(*highLatTrim * 180.0 / (double) d->m_geometry.height() - 90.0);
        axis.setValueCount(*highLatTrim - *lowLatTrim);
        axes->append(axis);
    }
    else {
        qDebug() << "Subset invalid.";
        calculateLonLimits(0, lowLatTrim, highLatTrim, axes, mode);
    }
}
