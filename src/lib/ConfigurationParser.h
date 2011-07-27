//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef CONFIGURATIONPARSER_H
#define CONFIGURATIONPARSER_H

#include <QtCore/QXmlStreamReader>
#include <QtCore/QHash>

class DataLayer;
class MapGeometry;
class ColorMap;
class QColor;

class ConfigurationParser : public QXmlStreamReader {
public:
    ConfigurationParser();
    ~ConfigurationParser();

    void read();

    QHash<QString,DataLayer *> layers() const;

    void setPath(const QString &path);
    QString path() const;
private:
    void readMap();
    void readUnknownElement();
    void readColorMap();
    QColor readColor();
    void readFiles(DataLayer *layer);
    MapGeometry readGeometry();
    DataLayer *readLayer();
    QString readCharacters();

    QString m_path;
    QHash<QString,DataLayer *> m_layers;
    QHash<QString,ColorMap> m_colorMaps;
};

#endif
