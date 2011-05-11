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
    
QTextStream *MapGeometryParser::textStream() const
{
    return m_textStream;
}

MapGeometry *MapGeometryParser::mapGeometry() const
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
                if(!ok || height <= 0) {
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
        return 0;
    }
    
    int sizes[height * width];
    int total = height * width;
    for(int i = 0; i < total; ++i) {
        int size;
        (*in) >> size;
        sizes[i] = size;
    }
    
    MapGeometry *geometry = new MapGeometry();
    geometry->setWidth(width);
    geometry->setHeight(height);
    geometry->setLayerCounts(sizes);
    return geometry;
}
