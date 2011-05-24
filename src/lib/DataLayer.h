//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DATALAYER_H
#define DATALAYER_H

class MapGeometry;
class QString;
class QFile;
class QDateTime;

class DataLayerPrivate;

class DataLayer {
public:
    DataLayer();
    virtual ~DataLayer();
    
    void setFileName(const QDateTime& dateTime, const QString& fileName);
    QString fileName(const QDateTime& dateTime) const;
    
    MapGeometry geometry() const;
    void setGeometry(const MapGeometry &mapGeometry);

private:
    Q_DISABLE_COPY(DataLayer);

    DataLayerPrivate * const d;
};

#endif
