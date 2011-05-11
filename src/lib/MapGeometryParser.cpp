//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>
#include <QtCore/QFile>

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
    m_file = file;
}

QFile *MapGeometryParser::file() const
{
    return m_file;
}

MapGeometry *MapGeometryParser::mapGeometry() const
{
    if(!m_file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return 0;
    }
    
    QTextStream in(m_file);
    int height = 0;
    int width = 0;
    while(!in.atEnd()) {
        if(in.readLine(1) == "#") {
            QString field;
            in >> field;
            if(field == "name:") {
                QString value;
                in >> value;
            }
            else if(field == "type:") {
                QString type;
                in >> type;
                if(type != "matrix") {
                    break;
                }
            }
            else if(field == "rows:") {
                QString rows;
                in >> rows;
                bool ok = true;
                height = rows.toInt(&ok);
                if(!ok || height <= 0) {
                    break;
                }
            }
            else if(field == "columns:") {
                QString columns;
                in >> columns;
                bool ok = true;
                width = columns.toInt(&ok);
                if(!ok || height <= 0) {
                    break;
                }
            }
        }
    }
    
    qDebug() << "Width:" << width << "height:" << height;
    
    return 0;
}
