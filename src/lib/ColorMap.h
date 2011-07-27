//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef COLORMAP_H
#define COLORMAP_H

// Qt
#include <QtGui/QColor>
#include <QtCore/QList>

class ColorMap {
public:
    ColorMap();
    ~ColorMap();
    
    void setInterpolationSpec(QColor::Spec spec);
    QColor::Spec interpolationSpec() const;
    
    void setColors(const QList<QColor>& colors);
    QList<QColor> colors() const;
    void addColor(const QColor& color);
    
    /**
     * Returns the interpolated color for the given @p value
     * between 0.0 and 1.0.
     */
    QColor color(qreal value);
    
private:
    QList<QColor> m_colors;
    QColor::Spec m_interpolationSpec;
};

#endif