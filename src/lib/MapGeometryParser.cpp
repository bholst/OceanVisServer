//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

// Project
#include "MapGeometry.h"

// Self
#include "MapGeometryParser.h"

MapGeometryParser::MapGeometryParser()
{   
}

MapGeometryParser::~MapGeometryParser()
{
}
    
void MapGeometryParser::setFile(QFile *file)
{
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream * fileStream = new QTextStream(file); 
    m_textStream = fileStream;
}

void MapGeometryParser::setTextStream(QTextStream *textStream)
{
    m_textStream = textStream;
}

void MapGeometryParser::setLayerSizesPath(const QString& layerSizesPath)
{
    m_layerSizesPath = layerSizesPath;
}
    
QTextStream *MapGeometryParser::textStream() const
{
    return m_textStream;
}

MapGeometry MapGeometryParser::mapGeometry() const
{
    QTextStream *in = m_textStream;
    int height = 0;
    int width = 0;
    while(!in->atEnd()) {
        if(in->readLine(1) == "#") {
            QString field;
            (*in) >> field;
            if(field == "name:") {
                QString value;
                (*in) >> value;
            }
            else if(field == "type:") {
                QString type;
                (*in) >> type;
                if(type != "matrix") {
                    break;
                }
            }
            else if(field == "rows:") {
                QString rows;
                (*in) >> rows;
                bool ok = true;
                height = rows.toInt(&ok);
                if(!ok || height <= 0) {
                    break;
                }
            }
            else if(field == "columns:") {
                QString columns;
                (*in) >> columns;
                bool ok = true;
                width = columns.toInt(&ok);
                if(!ok || width <= 0) {
                    break;
                }
            }
            in->readLine();
        }
        else {
            break;
        }
    }
    
    if(height * width <= 0) {
        // TODO: Throw exception.
        return MapGeometry();
    }

    int total = height * width;
    int sizes[total];
    for(int i = 0; i < total; ++i) {
        int size;
        (*in) >> size;
        sizes[i] = size;
    }
    
    MapGeometry geometry;
    geometry.setWidth(width);
    geometry.setHeight(height);
    geometry.setLayerCounts(sizes);
    
    if(!m_layerSizesPath.isEmpty()) {
        QFile *file = new QFile(m_layerSizesPath);
        if(!file->open(QIODevice::ReadOnly)) {
            qDebug() << "ERROR" << file->error() << ": File" << m_layerSizesPath << "cannot be opened.";
        }
        else {
            QList<double> layerSizes;
            QDataStream stream(file);
            double layerSize = 0.0;
            while(!stream.atEnd()) {
                stream >> layerSize;
                layerSizes.append(layerSize);
            }
            
            if(layerSizes.size() == geometry.maxLayerCount()) {
                qDebug() << "Successfully read" << layerSizes.size() << "depths";
                geometry.setLayerSizes(layerSizes);
            }
            else {
                qDebug() << "Depths given, but the number of depths is not correct";
                qDebug() << "Given" << layerSizes.size() << ", needed" << geometry.maxLayerCount();
            }
        }
    }
    
    return geometry;
}
