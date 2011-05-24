//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDateTime>
#include <QtCore/QMap>

// Project
#include "MapGeometry.h"

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
