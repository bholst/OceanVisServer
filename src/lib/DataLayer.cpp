//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <typeinfo>
#include <cmath>

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
#include "DataMatrix.h"

// Self
#include "DataLayer.h"

class DataLayerPrivate {
public:
    MapGeometry m_geometry;
    QMap<QDateTime, QFile *> m_files;
    QMap<QDateTime, double *> m_dataVectors;
};

DataLayer::DataLayer()
    : d(new DataLayerPrivate())
{
}

DataLayer::~DataLayer()
{
    delete d;
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
        qDebug() << "ERROR: File" << fileName << "cannot be opened.";
    }
    else {
        d->m_files.insert(dateTime, file);
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
            qDebug() << "Read" << dataVector[i];
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
    }
}

QString DataLayer::fileName(const QDateTime& dateTime) const
{
    QMap<QDateTime, QFile *>::const_iterator fileIterator = d->m_files.find(dateTime);
    if(fileIterator != d->m_files.end() && fileIterator.key() == dateTime) {
        return fileIterator.value()->fileName();
    }
    return QString();
}

DataMatrix *DataLayer::dataSubset(QList<DimensionSubset*>& subsets)
{
    QMap<Dimension,DimensionSlice> dimensionSlices;
    QMap<Dimension,DimensionTrim> dimensionTrims;
    for(int i = 0; i < subsets.size(); ++i) {
        DimensionSubset *subset = subsets[i];

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
        highTimeTrim = lowTimeTrim + 1;
        dimensionCount[0] = 1;
    }
    else if(timeTrimIt != dimensionTrims.constEnd()) {
        CoordinateAxis axis(Time);
        QDateTime trimLow = timeTrimIt->trimLow().toDateTime();
        QDateTime trimHigh = timeTrimIt->trimHigh().toDateTime();

        lowTimeTrim = d->m_dataVectors.lowerBound(trimLow);
        axis.setMinimumValue(lowTimeTrim.key());

        highTimeTrim = d->m_dataVectors.upperBound(trimHigh);
        if(lowTimeTrim == highTimeTrim) {
            highTimeTrim++;
        }

        for(QMap<QDateTime, double*>::const_iterator it = lowTimeTrim;
            it != highTimeTrim;
            ++it)
        {
            dimensionCount[0]++;
            axis.setMaximumValue(it.key());
        }
        axis.setValueCount(dimensionCount[0]);
        axes.append(axis);
    }
    else {
        lowTimeTrim = d->m_dataVectors.begin();
        highTimeTrim = d->m_dataVectors.end();
        dimensionCount[0] = d->m_dataVectors.size();
        CoordinateAxis axis(Time);
        axis.setMinimumValue(lowTimeTrim.key());
        axis.setMaximumValue((highTimeTrim - 1).key());
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

    if(lonSliceIt != dimensionSlices.end()) {
        int slicePoint = round(
            (lonSliceIt->slicePoint().toDouble() + 180.0) / 360.0 * (double) d->m_geometry.width());
        lowLonTrim = slicePoint;
        highLonTrim = slicePoint + 1;
        dimensionCount[1] = 1;
    }
    else if(lonTrimIt != dimensionTrims.end()) {
        lowLonTrim = std::ceil(
            (lonTrimIt->trimLow().toDouble() + 180.0) / 360.0 * (double) d->m_geometry.width());
        highLonTrim = std::floor(
            (lonTrimIt->trimHigh().toDouble() + 180.0) / 360.0 * (double) d->m_geometry.width()) + 1;
        dimensionCount[1] = highLonTrim - lowLonTrim;

        CoordinateAxis axis(Lon);
        axis.setMinimumValue(lowLonTrim * 360.0 / (double) d->m_geometry.width() - 180.0);
        axis.setMaximumValue((highLonTrim - 1) * 360.0 / (double) d->m_geometry.width() - 180.0);
        axis.setValueCount(dimensionCount[1]);
        axes.append(axis);
    }
    else {
        lowLonTrim = 0;
        highLonTrim = d->m_geometry.width();
        dimensionCount[1] = highLonTrim - lowLonTrim;

        CoordinateAxis axis(Lon);
        axis.setMinimumValue(-180.0);
        axis.setMaximumValue(+180.0);
        axis.setValueCount(dimensionCount[1]);
        axes.append(axis);
    }

    qDebug() << "Low lon trim:" << lowLonTrim;
    qDebug() << "High lon trim:" << highLonTrim;
    qDebug() << "Number of lon values:" << dimensionCount[1];

    QMap<Dimension,DimensionSlice>::const_iterator latSliceIt = dimensionSlices.find(Lat);
    QMap<Dimension,DimensionTrim>::const_iterator latTrimIt = dimensionTrims.find(Lat);
    int lowLatTrim = 0; // The first latitude value which will be in the returned matrix.
    int highLatTrim = 0; // The first latitude value which will not be in the returned matrix.

    if(latSliceIt != dimensionSlices.end()) {
        int slicePoint = round(
            (latSliceIt->slicePoint().toDouble() + 90.0) / 180.0 * (double) d->m_geometry.height());
        lowLatTrim = slicePoint;
        highLatTrim = slicePoint + 1;
        dimensionCount[2] = 1;
    }
    else if(latTrimIt != dimensionTrims.end()) {
        lowLatTrim = std::ceil(
            (latTrimIt->trimLow().toDouble() + 90.0) / 180.0 * (double) d->m_geometry.height());
        highLatTrim = std::floor(
            (latTrimIt->trimHigh().toDouble() + 90.0) / 180.0 * (double) d->m_geometry.height()) + 1;
        dimensionCount[2] = highLatTrim - lowLatTrim;

        CoordinateAxis axis(Lat);
        axis.setMinimumValue(-lowLatTrim * 180.0 / (double) d->m_geometry.height() + 90.0);
        axis.setMaximumValue(-(highLatTrim - 1) * 180.0 / (double) d->m_geometry.height() + 90.0);
        axis.setValueCount(dimensionCount[2]);
        axes.append(axis);
    }
    else {
        lowLatTrim = 0;
        highLatTrim = d->m_geometry.height();
        dimensionCount[2] = highLatTrim - lowLatTrim;

        CoordinateAxis axis(Lat);
        axis.setMinimumValue(-180.0);
        axis.setMaximumValue(+180.0);
        axis.setValueCount(dimensionCount[2]);
        axes.append(axis);
    }

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
            axis.setMinimumValue(lowHeightTrim * d->m_geometry.heightDimension());
            axis.setMaximumValue(highHeightTrim * d->m_geometry.heightDimension());
        }
        else {
            lowHeightTrim = std::ceil(
                heightTrimIt->trimHigh().toDouble() / d->m_geometry.heightDimension());
            highHeightTrim = std::floor(
                heightTrimIt->trimLow().toDouble() / d->m_geometry.heightDimension()) + 1;
            axis.setMinimumValue(highHeightTrim * d->m_geometry.heightDimension());
            axis.setMaximumValue(lowHeightTrim * d->m_geometry.heightDimension());
        }
        dimensionCount[3] = highHeightTrim - lowHeightTrim;
        axis.setValueCount(dimensionCount[3]);
        axes.append(axis);
    }
    else {
        lowLatTrim = 0;
        highLatTrim = d->m_geometry.maximumLayerCount();
        dimensionCount[3] = highLatTrim;

        CoordinateAxis axis(Height);
        if(d->m_geometry.heightDimension() >= 0) {
            axis.setMinimumValue(0.0);
            axis.setMaximumValue(d->m_geometry.maximumLayerCount() * d->m_geometry.heightDimension());
        }
        else {
            axis.setMinimumValue(d->m_geometry.maximumLayerCount() * d->m_geometry.heightDimension());
            axis.setMaximumValue(0.0);
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
}
