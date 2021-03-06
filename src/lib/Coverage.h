//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef COVERAGE_H
#define COVERAGE_H

#include <QtCore/QString>
#include <QtCore/QList>

// Project
#include "Constant.h"
#include "CoordinateAxis.h"
#include "ColorMap.h"

class Coverage {
public:
    Coverage();
    ~Coverage();
    
    void setCoverageId(const QString& coverageId);
    QString coverageId() const;
    
    void setConstants(const QList<Constant>& constants);
    QList<Constant> constants() const;
    
    void setCoordinateAxes(const QList<CoordinateAxis>& axes);
    QList<CoordinateAxis> coordinateAxes() const;
    
    void setColorMap(const ColorMap& colorMap);
    ColorMap colorMap() const;
    
    void setMaxValue(double maxValue);
    double maxValue() const;
    
    void setMinValue(double minValue);
    double minValue() const;
    
private:
    QString m_coverageId;
    QList<Constant> m_constants;
    QList<CoordinateAxis> m_axes;
    ColorMap m_colorMap;
    float m_minValue;
    float m_maxValue;
};

#endif
