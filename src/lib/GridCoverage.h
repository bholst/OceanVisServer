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

    QString toString() const;

    QImage toImage() const;
    QImage toImage(const QSize& size) const;
    QImage toImage(const QMap<Dimension, int>& sizes) const;

private:
    Q_DISABLE_COPY(GridCoverage)
    GridCoveragePrivate * const d;
};

#endif