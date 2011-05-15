//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QFile>

// Project
#include "MapGeometry.h"

// Self
#include "DataLayer.h"

DataLayer::DataLayer()
    : m_file(0)
{
}

DataLayer::~DataLayer()
{
    delete m_file;
    delete m_dataVector;
}
    
MapGeometry DataLayer::geometry() const
{
    return m_geometry;
}

void DataLayer::setGeometry(const MapGeometry &mapGeometry)
{
    m_geometry = mapGeometry;
}

void DataLayer::setFileName(const QString& fileName)
{
    m_file = new QFile(fileName);
    if(!m_file->open(QIODevice::ReadOnly)) {
        qDebug() << "ERROR: File" << fileName << "cannot be opened.";
        delete m_file;
        m_file = 0;
    }
    else {
        QDataStream stream(m_file);
        qint32 magicNumber;
        stream >> magicNumber;
        qDebug() << "The magic number is:" << magicNumber;
        stream >> m_fileLength;
        qDebug() << "The length is:" << m_fileLength;

        m_dataVector = new double[m_fileLength];
        for(int i = 0; i < m_fileLength; ++i) {
            if(stream.atEnd()) {
                qDebug() << "ERROR: file too short, missing data.";
            }
            stream >> m_dataVector[i];
            qDebug() << "Read" << m_dataVector[i];
        }
        if(!stream.atEnd()) {
            qDebug() << "ERROR: file to long, too much data.";
        }
    }
}

QString DataLayer::fileName() const
{
    if(m_file) {
        return m_file->fileName();
    }
    return QString();
}
