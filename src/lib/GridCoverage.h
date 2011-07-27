//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef GRIDCOVERAGE_H
#define GRIDCOVERAGE_H

// Qt
#include <QtCore/QList>
#include <QtCore/QMap>

// Project
#include "global.h"

class CoordinateAxis;
class ColorMap;
class QString;
class QImage;
class QSize;

class GridCoveragePrivate;

class GridCoverage {
public:
    GridCoverage();
    ~GridCoverage();

    void setCoordinateAxes(const QList<CoordinateAxis>& axes);
    QList<CoordinateAxis> coordinateAxes() const;

    /**
     * Set the value vector.
     * Data matrix will take the ownership of values.
     */
    void setValues(double *values);
    double *values();
    
    void setMinValue(double minValue);
    double minValue() const;

    void setMaxValue(double maxValue);
    double maxValue() const;
    
    void setName(const QString& name);
    QString name() const;
    
    void setColorMap(const ColorMap& colorMap);
    ColorMap colorMap() const;

    QString toString() const;

    QImage toImage(bool transparent = false) const;
    QImage toImage(const QSize& size, bool transparent = false) const;
    QImage toImage(const QMap<Dimension, int>& sizes, bool transparent = false) const;
    
    QImage toImage(CoordinateAxis xAxis, CoordinateAxis yAxis, bool transparent = false) const;

private:
    Q_DISABLE_COPY(GridCoverage)
    GridCoveragePrivate * const d;
};

#endif
