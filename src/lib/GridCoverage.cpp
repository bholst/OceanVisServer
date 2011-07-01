//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <cmath>

// Qt
#include <QtCore/QDebug>
#include <QtGui/QImage>
#include <QtGui/QColor>
#include <QtCore/QSize>

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
    double m_minValue;
    QString m_name;
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

void GridCoverage::setMinValue(double minValue)
{
    d->m_minValue = minValue;
}

double GridCoverage::minValue() const
{
    return d->m_minValue;
}

void GridCoverage::setMaxValue(double maxValue)
{
    d->m_maxValue = maxValue;
}

double GridCoverage::maxValue() const
{
    return d->m_maxValue;
}

void GridCoverage::setName(const QString& name)
{
    d->m_name = name;
}

QString GridCoverage::name() const
{
    return d->m_name;
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
    qDebug() << "Creating image (minValue =" << d->m_minValue << ", maxValue =" << d->m_maxValue;
    double span = d->m_maxValue - d->m_minValue;

    int width = d->m_axes[0].valueCount();
    int height = d->m_axes[1].valueCount();
    QImage result(width, height, QImage::Format_RGB32);
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            if(d->m_values[x * height + y] != d->m_values[x * height + y]) {
                // NaN
                result.setPixel(x, height - y - 1, 0);
            }
            else {
                double relative = (d->m_values[x * height + y] - d->m_minValue) / span;
                if(relative < 0.0)
                    relative = 0.0;
                if(relative > 1.0)
                    relative = 1.0;
            
                QColor color;
//                 color.setHsvF(relative, 1.0, 1.0);
                color.setHsvF(1.0, relative, 1.0);
            
                result.setPixel(x, height - y - 1, color.red() * 0x0100000 + color.green() * 0x000100 + color.blue() * 0x000001);
            }
        }
    }

    return result;
}

QImage GridCoverage::toImage(const QSize& size) const
{
    return toImage().scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

QImage GridCoverage::toImage(const QMap<Dimension, int>& sizes) const
{
    if(d->m_axes.length() != 2) {
        return QImage();
    }
    
    QSize size(d->m_axes.at(0).valueCount(), d->m_axes.at(1).valueCount());
    
    QMap<Dimension, int>::const_iterator it = sizes.constBegin();
    for(; it != sizes.constEnd(); ++it) {
        if(d->m_axes.at(0).dimension() == it.key()) {
            size.setWidth(it.value());
        }
        else if(d->m_axes.at(1).dimension() == it.key()) {
            size.setHeight(it.value());
        }
    }
    
    return toImage(size);
}
