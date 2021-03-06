//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QXmlStreamAttributes>
#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtGui/QColor>

// Project
#include "DataLayer.h"
#include "MapGeometry.h"
#include "MapGeometryParser.h"
#include "ColorMap.h"

// Self
#include "ConfigurationParser.h"

ConfigurationParser::ConfigurationParser()
{
}

ConfigurationParser::~ConfigurationParser()
{
}

void ConfigurationParser::read()
{
    qDebug() << "Read";
    QFile file(m_path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "File" << m_path << "cannot be opened";
        return;
    }
    QFileInfo fileInfo(file);
    fileInfo.makeAbsolute();
    QString oldCurrent = QDir::currentPath();
    QDir::setCurrent(fileInfo.absoluteDir().path());
    setDevice(&file);

    m_layers.clear();

    while(!atEnd()) {
        readNext();

        if(isStartElement()) {
            if(name() == "map") {
                readMap();
            }
            else {
                readUnknownElement();
            }
        }
    }

    QDir::setCurrent(oldCurrent);
    qDebug() << "Finished reading configuration.";
}

void ConfigurationParser::readMap()
{
    Q_ASSERT(isStartElement()
              && name() == "map");

    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement()) {
            if(name() == "geometry") {
                readGeometry();
            }
            else if (name() == "layer") {
                DataLayer *layer = readLayer();
                if(layer) {
                    m_layers.insert(layer->name(), layer);
                }
            }
            else if (name() == "colorMap") {
                ColorMap colorMap = ColorMap::readColorMap(this);
                m_colorMaps.insert(colorMap.name(), colorMap);
            }
            else {
                readUnknownElement();
            }
        }
    }
}

void ConfigurationParser::readGeometry()
{
    Q_ASSERT(isStartElement()
              && name() == "geometry");

    QString path;
    double heightDimension = 1.0;
    QString geometryName = attributes().value("name").toString();
    QString layerSizesPath;

    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement()) {
            if(name() == "layers") {
                path = readCharacters();
            }
            else if(name() == "heightDimension") {
                heightDimension = readCharacters().toDouble();
            }
            else if(name() == "depths") {
                layerSizesPath = readCharacters();
            }
            else {
                readUnknownElement();
            }
        }
    }

    QFile file(path);

    MapGeometryParser parser;
    parser.setFile(&file);
    parser.setLayerSizesPath(layerSizesPath);
    
    MapGeometry geometry = parser.mapGeometry();
    geometry.setHeightDimension(heightDimension);
    m_geometries.insert(geometryName, geometry);
}

DataLayer *ConfigurationParser::readLayer()
{
    Q_ASSERT(isStartElement()
              && name() == "layer");

    DataLayer *layer = new DataLayer();
    ColorMap colorMap;
    
    QColor c;
    c.setHsv(240, 255, 189, 255);
    colorMap.addColor(c);
    c.setHsv(240, 255, 255, 255);
    colorMap.addColor(c);
    c.setHsv(224, 255, 255, 255);
    colorMap.addColor(c);
    c.setHsv(208, 255, 255, 255);
    colorMap.addColor(c);
    c.setHsv(195, 255, 255, 255);
    colorMap.addColor(c);
    c.setHsv(180, 255, 255, 255);
    colorMap.addColor(c);
    c.setHsv(159, 189, 255, 255);
    colorMap.addColor(c);
    c.setHsv(120, 123, 255, 255);
    colorMap.addColor(c);
    c.setHsv(80, 189, 255, 255);
    colorMap.addColor(c);
    c.setHsv(60, 255, 255, 255);
    colorMap.addColor(c);
    c.setHsv(44, 255, 255, 255);
    colorMap.addColor(c);
    c.setHsv(15, 255, 255, 255);
    colorMap.addColor(c);
    c.setHsv(0, 255, 255, 255);
    colorMap.addColor(c);
    c.setHsv(0, 255, 189, 255);
    colorMap.addColor(c);
    c.setHsv(0, 255, 132, 255);
    colorMap.addColor(c);
    colorMap.setInterpolationSpec(QColor::Hsv);

    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement()) {
            if(name() == "file") {
                QXmlStreamAttributes att = attributes();
                QDateTime dateTime;
                dateTime.setTime(QTime::fromString(att.value("time").toString(), Qt::ISODate));
                dateTime.setDate(QDate::fromString(att.value("date").toString(), Qt::ISODate));
                layer->setFileName(dateTime, readCharacters());
                qDebug() << "Added file for time:" << dateTime;
            }
            else if(name() == "files") {
                readFiles(layer);
            }
            else if(name() == "name") {
                layer->setName(readCharacters());
                qDebug() << "Name:" << layer->name();
            }
            else if(name() == "geometry") {
                QString geometryName = readCharacters();
                
                QHash<QString,MapGeometry>::iterator it = m_geometries.find(geometryName);
                if(it != m_geometries.end()) {
                    layer->setGeometry(it.value());
                }
                else {
                    delete layer;
                    qDebug() << "Geometry not found.";
                    return 0;
                }
            }
            else if(name() == "scaleMin") {
                layer->setScaleMin(readCharacters().toDouble());
            }
            else if(name() == "scaleMax") {
                layer->setScaleMax(readCharacters().toDouble());
            }
            else if(name() == "defaultColorMap") {
                QString colorMapString = readCharacters();
                if(m_colorMaps.contains(colorMapString)) {
                    colorMap = m_colorMaps.value(colorMapString);
                }
            }
            else {
                readUnknownElement();
            }
        }
    }

    layer->setDefaultColorMap(colorMap);
    return layer;
}



void ConfigurationParser::readFiles(DataLayer *layer)
{
    Q_ASSERT(isStartElement()
             && name() == "files");
    
    QDateTime startDate;
    int start = 0;
    QDateTime endDate;
    int end = 0;
    int digits = 0;
    QString scheme;
    int skip = 0;
    
    while(!atEnd()) {
        readNext();
        
        if(isEndElement()) {
            break;
        }
        
        if(isStartElement()) {
            if(name() == "start") {
                QXmlStreamAttributes att = attributes();
                startDate.setTime(QTime::fromString(att.value("time").toString(), Qt::ISODate));
                startDate.setDate(QDate::fromString(att.value("date").toString(), Qt::ISODate));
                start = readCharacters().toInt();
            }
            else if(name() == "end") {
                QXmlStreamAttributes att = attributes();
                endDate.setTime(QTime::fromString(att.value("time").toString(), Qt::ISODate));
                endDate.setDate(QDate::fromString(att.value("date").toString(), Qt::ISODate));
                end = readCharacters().toInt();
            }
            else if(name() == "scheme") {
                QXmlStreamAttributes att = attributes();
                digits = att.value("digits").toString().toInt();
                if(att.hasAttribute("skip")) {
                    skip = att.value("skip").toString().toInt();
                }
                scheme = readCharacters();
            }
            else {
                readUnknownElement();
            }
        }
    }
    
    if(startDate.isValid() && endDate.isValid()) {
        qint64 spanMSecs = endDate.toMSecsSinceEpoch() - startDate.toMSecsSinceEpoch();
        int count = end - start;
        qint64 diffMSecs = spanMSecs / count;
        
        for(int i = start; i < end; i += 1 + skip) {
            QDateTime fileTime = startDate.addMSecs(diffMSecs * (i - start));
            qDebug() << "Time [" << i << "]:" << fileTime.toString();
            QString fileName = scheme.arg(i, digits, 10, QChar('0'));
            
            layer->setFileName(fileTime, fileName);
        }
    }
}

QHash<QString,DataLayer *> ConfigurationParser::layers() const
{
    return m_layers;
}

void ConfigurationParser::setPath(const QString &path)
{
    m_path = path;
}

QString ConfigurationParser::path() const
{
    return m_path;
}
