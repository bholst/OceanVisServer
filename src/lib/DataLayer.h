//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DATALAYER_H
#define DATALAYER_H

#include <QtCore/QDateTime>
#include <QtCore/QMap>

class MapGeometry;
class QString;
class QFile;

class DataLayer {
public:
    DataLayer();
    virtual ~DataLayer();
    
    void setFileName(const QDateTime& dateTime, const QString& fileName);
    QString fileName(const QDateTime& dateTime) const;
    
    MapGeometry geometry() const;
    void setGeometry(const MapGeometry &mapGeometry);

private:
    MapGeometry m_geometry;
    QMap<QDateTime, QFile *> m_files;
    QMap<QDateTime, double *> m_dataVectors;
};

#endif
