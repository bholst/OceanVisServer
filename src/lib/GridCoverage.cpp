//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <cmath>

// Qt
#include <QtCore/QDebug>
#include <QtGui/QImage>

// Project
#include "CoordinateAxis.h"

// Self
#include "GridCoverage.h"

const double default_max_value = 5.0;

class GridCoveragePrivate {
public:
    GridCoveragePrivate()
        : m_values(0),
          m_maxValue(default_max_value)
    {
    }

    ~GridCoveragePrivate()
    {
        delete m_values;
    }

    void inStringUntilDimension(int until,
                                int offset,
                                int higherLevelCount,
                                QString& string) const;

    QList<CoordinateAxis> m_axes;
    double *m_values;
    double m_maxValue;
};

void GridCoveragePrivate::inStringUntilDimension(int until,
                                               int offset,
                                               int higherLevelCount,
                                               QString& string) const
{
    if(until < 0) {
        string += QString("%1").arg(m_values[offset], 10);
        return;
    }
    CoordinateAxis axis = m_axes[until];

    int begin = 0;
    int end = axis.valueCount();
    int incrementor = 1;
    int nextHigherLevelCount = higherLevelCount * axis.valueCount();
    if(until == 1) {
        begin = axis.valueCount() - 1;
        end = - 1;
        incrementor = -1;
    }

    for(int i = begin; i != end; i += incrementor) {
        inStringUntilDimension(until - 1,
                               (i * higherLevelCount) + offset,
                               nextHigherLevelCount,
                               string);
        if(until == 0) {
            string += " ";
        }
        for(int j = 0; j < until; ++j) {
            string += "\n";
        }
    }
}

GridCoverage::GridCoverage()
    : d(new GridCoveragePrivate())
{
}

GridCoverage::~GridCoverage()
{
    delete d;
}

void GridCoverage::setCoordinateAxes(const QList<CoordinateAxis>& axes)
{
    d->m_axes = axes;
}

QList<CoordinateAxis> GridCoverage::coordinateAxes() const
{
    return d->m_axes;
}

void GridCoverage::setValues(double *values)
{
    delete d->m_values;
    d->m_values = values;
}

double *GridCoverage::values()
{
    return d->m_values;
}

void GridCoverage::setMaxValue(double maxValue)
{
    d->m_maxValue = maxValue;
}

double GridCoverage::maxValue()
{
    return d->m_maxValue;
}

QString GridCoverage::toString() const
{
    QString result;
    d->inStringUntilDimension(d->m_axes.length() - 1, 0, 1, result);
    return result;
}

QImage GridCoverage::toImage() const
{
    if(d->m_axes.length() != 2) {
        return QImage();
    }

    int width = d->m_axes[0].valueCount();
    int height = d->m_axes[1].valueCount();
    QImage result(width, height, QImage::Format_RGB32);
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            result.setPixel(x, height - y - 1, 0x010000 * round((d->m_values[x * height + y] / d->m_maxValue) * 355));
        }
    }

    return result;
}
