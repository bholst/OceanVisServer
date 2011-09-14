//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QAtomicInt>
#include <QtCore/QVariant>
#include <QtCore/QDebug>

// Project
#include "Dimension.h"

// Self
#include "CoordinateAxis.h"

class CoordinateAxisPrivate {
public:
    CoordinateAxisPrivate(Dimension dimension)
        : m_dimension(dimension),
          m_valueCount(0),
          ref(1)
    {
    }

    CoordinateAxisPrivate(const CoordinateAxisPrivate& other)
        : m_dimension(other.m_dimension),
          m_lowerLimit(other.m_lowerLimit),
          m_upperLimit(other.m_upperLimit),
          m_values(other.m_values),
          m_valueCount(other.m_valueCount),
          ref(1)
    {
    }

    ~CoordinateAxisPrivate()
    {
    }

    /*
     * Return this instead of other.
     */
    CoordinateAxisPrivate& operator=(const CoordinateAxisPrivate &other)
    {
        m_dimension = other.m_dimension;
        m_lowerLimit = other.m_lowerLimit;
        m_upperLimit = other.m_upperLimit;
        m_values = other.m_values;
        m_valueCount = other.m_valueCount;
        ref = other.ref;
        return *this;
    }

    bool operator==(const CoordinateAxisPrivate &other)
    {
        return m_dimension == other.m_dimension
               && m_lowerLimit == other.m_lowerLimit
               && m_upperLimit == other.m_upperLimit
               && m_values == other.m_values
               && m_valueCount == other.m_valueCount;
    }

    Dimension m_dimension;
    QVariant m_lowerLimit;
    QVariant m_upperLimit;
    QList<QVariant> m_values;
    int m_valueCount;
    QAtomicInt ref;
};

CoordinateAxis::CoordinateAxis(Dimension dimension)
    : d(new CoordinateAxisPrivate(dimension))
{
}

CoordinateAxis::CoordinateAxis(const CoordinateAxis &other)
{
    this->d = other.d;
    this->d->ref.ref();
}

CoordinateAxis::~CoordinateAxis()
{
    if(!d->ref.deref())
        delete d;
}

Dimension CoordinateAxis::dimension() const
{
    return d->m_dimension;
}

void CoordinateAxis::setLowerLimit(const QVariant& lowerLimit) throw (BadDimensionTypeException)
{
    checkDimensionType(d->m_dimension, lowerLimit);
    detach();
    d->m_lowerLimit = lowerLimit;
}

QVariant CoordinateAxis::lowerLimit() const
{
    return d->m_lowerLimit;
}

void CoordinateAxis::setUpperLimit(const QVariant& upperLimit) throw (BadDimensionTypeException)
{
    checkDimensionType(d->m_dimension, upperLimit);
    detach();
    d->m_upperLimit = upperLimit;
}

QVariant CoordinateAxis::upperLimit() const
{
    return d->m_upperLimit;
}

void CoordinateAxis::setValues(const QList<QVariant>& values) throw (BadDimensionTypeException)
{
    foreach(QVariant value, values) {
        checkDimensionType(d->m_dimension, value);
    }
    detach();
    d->m_values = values;
}

QList<QVariant> CoordinateAxis::values() const
{
    return d->m_values;
}

void CoordinateAxis::setValueCount(int valueCount)
{
    detach();
    d->m_valueCount = valueCount;
}

int CoordinateAxis::valueCount() const
{
    return d->m_valueCount;
}

CoordinateAxis& CoordinateAxis::operator=(const CoordinateAxis &other)
{
    qAtomicAssign(d, other.d);
    return *this;
}

bool CoordinateAxis::operator==(const CoordinateAxis &other)
{
    return d == other.d || (*d) == (*other.d);
}

void CoordinateAxis::detach()
{
    qAtomicDetach(d);
}
