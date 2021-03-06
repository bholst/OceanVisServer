//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef COLORMAPWIDGET_H
#define COLORMAPWIDGET_H

// Qt
#include <QtGui/QWidget>

class ColorMap;

class ColorMapWidget : public QWidget {
    Q_OBJECT

public:
    ColorMapWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    
    ColorMap colorMap() const;
    
public slots:
    void setColorMap(const ColorMap& colorMap);
    void setMinValue(double minValue);
    void setMaxValue(double maxValue);
    
protected:
    void paintEvent(QPaintEvent *event);
    
private:
    ColorMap m_colorMap;
    double m_minValue;
    double m_maxValue;
};

#endif