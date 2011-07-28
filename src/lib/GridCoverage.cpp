//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <cmath>

// Qt
#include <QtCore/QDebug>
#include <QtGui/QImage>
#include <QtGui/QColor>
#include <QtGui/QTransform>
#include <QtCore/QSize>

// Project
#include "ColorMap.h"
#include "CoordinateAxis.h"
#include "Constant.h"

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
    QList<Constant> m_consts;
    double *m_values;
    double m_maxValue;
    double m_minValue;
    QString m_name;
    ColorMap m_colorMap;
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

void GridCoverage::setConstants(const QList<Constant>& consts)
{
    d->m_consts = consts;
}

QList<Constant> GridCoverage::constants() const
{
    return d->m_consts;
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

void GridCoverage::setColorMap(const ColorMap& colorMap)
{
    d->m_colorMap = colorMap;
}

ColorMap GridCoverage::colorMap() const
{
    return d->m_colorMap;
}

QString GridCoverage::toString() const
{
    QString result;
    d->inStringUntilDimension(d->m_axes.length() - 1, 0, 1, result);
    return result;
}

QImage GridCoverage::toImage(bool transparent) const
{
    if(d->m_axes.length() != 2) {
        return QImage();
    }
    qDebug() << "Creating image (minValue =" << d->m_minValue << ", maxValue =" << d->m_maxValue;
    double span = d->m_maxValue - d->m_minValue;
    
    int width = d->m_axes[0].valueCount();
    int height = d->m_axes[1].valueCount();
    QImage result(width, height, QImage::Format_ARGB32);
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            if(d->m_values[x * height + y] != d->m_values[x * height + y]) {
                // NaN
                if(transparent) {
                    result.setPixel(x, height - y - 1, qRgba(0, 0, 0, 0));
                }
                else {
                    result.setPixel(x, height - y - 1, qRgba(0, 0, 0, 255));
                }
            }
            else {
                double relative = (d->m_values[x * height + y] - d->m_minValue) / span;
                if(relative < 0.0)
                    relative = 0.0;
                if(relative > 1.0)
                    relative = 1.0;
            
                QColor color;
//                 color.setHsvF(relative, 1.0, 1.0);
//                 color.setHsvF(1.0, relative, 1.0);
                color.setRgbF(1.0, 0, 0);
            
                result.setPixel(x, height - y - 1, d->m_colorMap.color(relative).rgba());
            }
        }
    }

    return result;
}

QImage GridCoverage::toImage(const QSize& size, bool transparent) const
{
    return toImage(transparent).scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

QImage GridCoverage::toImage(const QMap<Dimension, int>& sizes, bool transparent) const
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
    
    return toImage(size, transparent);
}

QImage GridCoverage::toImage(CoordinateAxis xAxis, CoordinateAxis yAxis, bool transparent) const
{
    if(d->m_axes.length() != 2) {
        return QImage();
    }
    
    if(d->m_axes.at(0).dimension() == Time
       || d->m_axes.at(1).dimension() == Time)
    {
        qDebug() << "Exact scaling currently not supported for the time axis.";
        return QImage();
    }
    
    if(d->m_axes.at(0).dimension() != xAxis.dimension()) {
        qDebug() << "Wrong dimension of given x-axis";
        return QImage();
    }
    if(d->m_axes.at(1).dimension() != yAxis.dimension()) {
        qDebug() << "Wrong dimension of given y-axis";
        return QImage();
    }

    QImage originalImage = toImage(transparent);
    
    QTransform transform;
    qreal translateX = (xAxis.lowerLimit().toReal() - d->m_axes.at(0).lowerLimit().toReal())
                       / (d->m_axes.at(0).upperLimit().toReal() - d->m_axes.at(0).lowerLimit().toReal())
                       * (qreal) d->m_axes.at(0).valueCount();
    qreal translateY = (d->m_axes.at(1).upperLimit().toReal() - yAxis.upperLimit().toReal())
                       / (d->m_axes.at(1).upperLimit().toReal() - d->m_axes.at(1).lowerLimit().toReal())
                       * (qreal) d->m_axes.at(1).valueCount();
    qDebug() << "translate(" << translateX << "," << translateY << ")";
    transform.translate(translateX, translateY);
    
    qreal scaleX = ((qreal) xAxis.valueCount() / (qreal) d->m_axes.at(0).valueCount())
                   * ((d->m_axes.at(0).upperLimit().toReal() - d->m_axes.at(0).lowerLimit().toReal()) / (xAxis.upperLimit().toReal() - xAxis.lowerLimit().toReal()));
    
    qreal scaleY = ((qreal) yAxis.valueCount() / (qreal) d->m_axes.at(1).valueCount())
                   * ((d->m_axes.at(1).upperLimit().toReal() - d->m_axes.at(1).lowerLimit().toReal()) / (yAxis.upperLimit().toReal() - yAxis.lowerLimit().toReal()));

    qDebug() << "scale(" << scaleX << "," << scaleY << ")";
    transform.scale(scaleX, scaleY);
    
    return originalImage.transformed(transform).copy(0, 0, xAxis.valueCount(), yAxis.valueCount());
}
    
    