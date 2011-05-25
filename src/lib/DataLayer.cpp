//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <typeinfo>

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
    }

    qDebug() << "Low time trim:" << lowTimeTrim.key().toString();
    if(highTimeTrim != d->m_dataVectors.constEnd()) {
        qDebug() << "High time trim:" << highTimeTrim.key().toString();
    }
}
