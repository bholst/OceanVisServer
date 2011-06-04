//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef CONFIGURATIONPARSER_H
#define CONFIGURATIONPARSER_H

#include <QtCore/QXmlStreamReader>
#include <QtCore/QList>

class DataLayer;
class MapGeometry;

class ConfigurationParser : public QXmlStreamReader {
public:
    ConfigurationParser();
    ~ConfigurationParser();

    void read();

    QList<DataLayer *> layers() const;

    void setPath(const QString &path);
    QString path() const;
private:
    void readMap();
    void readUnknownElement();
    MapGeometry readGeometry();
    DataLayer *readLayer();
    QString readCharacters();

    QString m_path;
    QList<DataLayer *> m_layers;
};

#endif
