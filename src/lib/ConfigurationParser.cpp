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
            if(name() == "map")
                readMap();
        }
    }

    QDir::setCurrent(oldCurrent);
    qDebug() << "Finished reading configuration.";
}

void ConfigurationParser::readMap()
{
    Q_ASSERT(isStartElement()
              && name() == "map");

    MapGeometry geometry;

    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement()) {
            if(name() == "geometry") {
                geometry = readGeometry();
            }
            else if (name() == "layer") {
                DataLayer *layer = readLayer();
                m_layers.insert(layer->name(), layer);
            }
            else if (name() == "colorMap") {
                readColorMap();
            }
            else {
                readUnknownElement();
            }
        }
    }

    foreach(DataLayer *layer, m_layers) {
        layer->setGeometry(geometry);
    }
}

void ConfigurationParser::readUnknownElement()
{
    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement())
            readUnknownElement();
    }
}

MapGeometry ConfigurationParser::readGeometry()
{
    Q_ASSERT(isStartElement()
              && name() == "geometry");

    QString path;
    double heightDimension = 1.0;

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
            else {
                readUnknownElement();
            }
        }
    }

    QFile file(path);

    MapGeometryParser parser;
    parser.setFile(&file);
    
    MapGeometry geometry = parser.mapGeometry();
    geometry.setHeightDimension(heightDimension);
    return geometry;
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

void ConfigurationParser::readColorMap()
{
    Q_ASSERT(isStartElement()
             && name() == "colorMap");
    
    ColorMap colorMap;
    QString mapName = attributes().value("name").toString();
    
    while(!atEnd()) {
        readNext();
        
        if(isEndElement()) {
            break;
        }

        if(isStartElement()) {
            QString n = name().toString();
            if(name() == "color") {
                colorMap.addColor(readColor());
            }
            else if(name() == "interpolationSpec") {
                QString intSpec = readCharacters();
                if(intSpec == "rgb") {
                    colorMap.setInterpolationSpec(QColor::Rgb);
                }
                else if(intSpec == "hsv") {
                    colorMap.setInterpolationSpec(QColor::Hsv);
                }
                else if(intSpec == "cmyk") {
                    colorMap.setInterpolationSpec(QColor::Cmyk);
                }
                else if(intSpec == "hsl") {
                    colorMap.setInterpolationSpec(QColor::Hsl);
                }
            }
            else {
                readUnknownElement();
            }
        }
    }
    
    m_colorMaps.insert(mapName, colorMap);
}

QColor ConfigurationParser::readColor()
{
    Q_ASSERT(isStartElement()
             && name() == "color");

    QColor::Spec spec = QColor::Rgb;
    QXmlStreamAttributes colorAtt = attributes();
    if(colorAtt.hasAttribute("spec")) {
        QString specString = colorAtt.value("spec").toString();
        if(specString == "rgb") {
            spec = QColor::Rgb;
        }
        else if(specString == "hsv") {
            spec = QColor::Hsv;
        }
        else if(specString == "cmyk") {
            spec = QColor::Cmyk;
        }
        else if(specString == "hsl") {
            spec = QColor::Hsl;
        }
    }
    
    QString colorStr = readCharacters();
    QStringList colorStrings = colorStr.split(',', QString::SkipEmptyParts);
    QList<int> colorValues;
    foreach(QString colorString, colorStrings) {
        colorValues.append(colorString.toInt());
    }
    QColor color;
    
    if(colorValues.size() == 3) {
        switch(spec) {
            case QColor::Rgb:
                color.setRgb(colorValues[0], colorValues[1], colorValues[2]);
                break;
            case QColor::Hsv:
                color.setHsv(colorValues[0], colorValues[1], colorValues[2]);
                break;
            case QColor::Hsl:
                color.setHsl(colorValues[0], colorValues[1], colorValues[2]);
                break;
            default:
                qDebug() << "Color: Spec and number of values don't agree.";
                break;
        }
    }
    else if(colorValues.size() == 4) {
        switch(spec) {
            case QColor::Rgb:
                color.setRgb(colorValues[0], colorValues[1], colorValues[2], colorValues[3]);
                break;
            case QColor::Hsv:
                color.setHsv(colorValues[0], colorValues[1], colorValues[2], colorValues[3]);
                break;
            case QColor::Hsl:
                color.setHsl(colorValues[0], colorValues[1], colorValues[2], colorValues[3]);
                break;
            case QColor::Cmyk:
                color.setCmyk(colorValues[0], colorValues[1], colorValues[2], colorValues[3]);
                break;
            default:
                qDebug() << "Color: Spec and number of values don't agree.";
                break;
        }
    }
    else if(colorValues.size() == 5 && spec == QColor::Cmyk) {
        color.setCmyk(colorValues[0], colorValues[1], colorValues[2], colorValues[3], colorValues[4]);
    }
    else {
        color.setCmyk(colorValues[0], colorValues[1], colorValues[2], colorValues[3]);
    }
    
    return color;
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
    int startIndex = 0;
    
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
                if(att.hasAttribute("start")) {
                    startIndex = att.value("start").toString().toInt();
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
        qint64 diffMSecs = spanMSecs / (count + 1);
        
        for(int i = startIndex; i < count; i += 1 + skip) {
            QDateTime fileTime = startDate.addMSecs(diffMSecs * i);
            QString fileName = scheme.arg(start + i, digits, 10, QChar('0'));
            
            layer->setFileName(fileTime, fileName);
        }
    }
}

QString ConfigurationParser::readCharacters()
{
    Q_ASSERT(isStartElement());

    QString string;

    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement()) {
            readUnknownElement();
        }

        if(isCharacters()) {
            string = text().toString();
        }
    }

    return string;
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
