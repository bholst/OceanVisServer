//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DATALAYER_H
#define DATALAYER_H

class MapGeometry;
class QString;
class QFile;

class DataLayer {
public:
    DataLayer();
    virtual ~DataLayer();
    
    void setFileName(const QString& fileName);
    QString fileName() const;
    
    MapGeometry geometry() const;
    void setGeometry(const MapGeometry &mapGeometry);

private:
    MapGeometry m_geometry;
    QFile *m_file;
    qint32 m_fileLength;
};

#endif
