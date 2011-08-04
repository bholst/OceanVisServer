//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef CONFIGURATIONPARSER_H
#define CONFIGURATIONPARSER_H

#include <QtCore/QHash>

#include "XmlStreamReader.h"

class DataLayer;
class MapGeometry;
class ColorMap;
class QColor;

class ConfigurationParser : public XmlStreamReader {
public:
    ConfigurationParser();
    ~ConfigurationParser();

    void read();

    QHash<QString,DataLayer *> layers() const;

    void setPath(const QString &path);
    QString path() const;
private:
    void readMap();
    QColor readColor();
    void readFiles(DataLayer *layer);
    MapGeometry readGeometry();
    DataLayer *readLayer();

    QString m_path;
    QHash<QString,DataLayer *> m_layers;
    QHash<QString,ColorMap> m_colorMaps;
};

#endif
