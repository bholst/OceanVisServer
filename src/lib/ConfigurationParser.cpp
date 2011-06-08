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

// Project
#include "DataLayer.h"
#include "MapGeometry.h"
#include "MapGeometryParser.h"

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

    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement()) {
            if(name() == "layers") {
                path = readCharacters();
            }
            else {
                readUnknownElement();
            }
        }
    }

    QFile file(path);

    MapGeometryParser parser;
    parser.setFile(&file);
    return parser.mapGeometry();
}

DataLayer *ConfigurationParser::readLayer()
{
    Q_ASSERT(isStartElement()
              && name() == "layer");

    DataLayer *layer = new DataLayer();

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
            else if(name() == "name") {
                layer->setName(readCharacters());
                qDebug() << "Name:" << layer->name();
            }
            else {
                readUnknownElement();
            }
        }
    }

    return layer;
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
