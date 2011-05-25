//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Project
#include "CoordinateAxis.h"

// Self
#include "DataMatrix.h"

class DataMatrixPrivate {
public:
    QList<CoordinateAxis> m_axes;
    double *m_values;
};

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
