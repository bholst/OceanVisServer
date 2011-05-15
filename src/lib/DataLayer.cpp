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
    }
}

QString DataLayer::fileName() const
{
    if(m_file) {
        return m_file->fileName();
    }
    return QString();
}
