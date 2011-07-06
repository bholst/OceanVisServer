//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef GETMAP_H
#define GETMAP_H

// Qt
#include <QtCore/QList>
#include <QtCore/QDateTime>
#include <QtGui/QColor>

// Project
#include "global.h"
#include "RequestBase.h"
#include "BoundingBox.h"
#include "DimensionSubset.h"

class GetMap : public RequestBase
{
public:
    GetMap();
    virtual ~GetMap();
    
    virtual QString request() const;
    
    QList<DimensionSubset*> dimensionSubsets() const;
    
    QStringList layers() const;
    void setLayers(const QStringList& layers);
    void addLayer(const QString& layer);
    
    QStringList styles() const;
    void setStyles(const QStringList& styles);
    void addStyle(const QString& style);
    
    BoundingBox boundingBox() const;
    void setBoundingBox(const BoundingBox& boundingBox);
    
    int width() const;
    void setWidth(int width);
    
    int height() const;
    void setHeight(int height);
    
    /**
     * Set the return format. Possible formats are for example:
     * text/xml
     * image/png
     */
    void setFormat(const QString& format);
    
    /**
     * Return the return format.
     */
    QString format() const;
    
    bool transparent() const;
    void setTransparent(bool transparent);
    
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor& backgroundColor);
    
    QDateTime time() const;
    void setTime(const QDateTime& time);
    
    int elevation() const;
    void setElevation(int elevation);
    
    static GetMap *fromRequestString(QString request);
    
    QString toString() const;
    
private:
    QStringList m_layers;
    QStringList m_styles;
    BoundingBox m_boundingBox;
    int m_width;
    int m_height;
    QString m_format;
    bool m_transparent;
    QColor m_backgroundColor;
    QDateTime m_time;
    int m_elevation;
    mutable QList<DimensionSubset *> m_dimensionSubsets;
    mutable bool m_dimensionSubsetsChanged;
};

#endif
