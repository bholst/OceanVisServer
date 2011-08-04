//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef COLORMAP_H
#define COLORMAP_H

// Qt
#include <QtGui/QColor>
#include <QtCore/QList>

class XmlStreamReader;
class QXmlStreamWriter;
class QColor;

class ColorMap {
public:
    ColorMap();
    ~ColorMap();
    
    void setInterpolationSpec(QColor::Spec spec);
    QColor::Spec interpolationSpec() const;
    
    void setColors(const QList<QColor>& colors);
    QList<QColor> colors() const;
    void addColor(const QColor& color);
    
    void setName(const QString& name);
    QString name() const;
    
    /**
     * Returns the interpolated color for the given @p value
     * between 0.0 and 1.0.
     */
    QColor color(qreal value);
    
    static ColorMap readColorMap(XmlStreamReader *reader);
    void writeColorMap(QXmlStreamWriter *writer);
    
private:
    static QColor readColor(XmlStreamReader *reader);
    void writeColor(QColor color, QXmlStreamWriter *writer);
    
    QList<QColor> m_colors;
    QColor::Spec m_interpolationSpec;
    QString m_name;
};

#endif