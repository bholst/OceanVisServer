//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Project
#include "CoordinateAxis.h"

// Self
#include "DataMatrix.h"

class DataMatrixPrivate {
public:
    DataMatrixPrivate()
        : m_values(0)
    {
    }

    ~DataMatrixPrivate()
    {
        delete m_values;
    }

    void inStringUntilDimension(int until,
                                int offset,
                                int higherLevelCount,
                                QString& string) const;

    QList<CoordinateAxis> m_axes;
    double *m_values;
};

void DataMatrixPrivate::inStringUntilDimension(int until,
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
    int end = 0;
    int incrementor = 0;
    int nextHigherLevelCount = higherLevelCount * axis.valueCount();
    switch (axis.dimension()) {
        case Height:
        case Lat:
            begin = axis.valueCount() - 1;
            end = - 1;
            incrementor = -1;
            break;
        default:
            begin = 0;
            end = axis.valueCount();
            incrementor = 1;
            break;
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

DataMatrix::DataMatrix()
    : d(new DataMatrixPrivate())
{
}

DataMatrix::~DataMatrix()
{
    delete d;
}

void DataMatrix::setCoordinateAxes(const QList<CoordinateAxis>& axes)
{
    d->m_axes = axes;
}

QList<CoordinateAxis> DataMatrix::coordinateAxes() const
{
    return d->m_axes;
}

void DataMatrix::setValues(double *values)
{
    d->m_values = values;
}

double *DataMatrix::values()
{
    return d->m_values;
}

QString DataMatrix::toString() const
{
    QString result;
    d->inStringUntilDimension(d->m_axes.length() - 1, 0, 1, result);
    return result;
}
