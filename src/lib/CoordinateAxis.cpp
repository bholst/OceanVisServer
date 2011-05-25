//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QAtomicInt>
#include <QtCore/QVariant>

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
          m_minimumValue(other.m_minimumValue),
          m_maximumValue(other.m_maximumValue),
          m_valueCount(other.m_valueCount),
          ref(other.ref)
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
        m_minimumValue = other.m_minimumValue;
        m_maximumValue = other.m_maximumValue;
        m_valueCount = other.m_valueCount;
        ref = other.ref;
        return *this;
    }

    Dimension m_dimension;
    QVariant m_minimumValue;
    QVariant m_maximumValue;
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

void CoordinateAxis::setMinimumValue(const QVariant& minimumValue) throw (BadDimensionTypeException)
{
    checkDimensionType(d->m_dimension, minimumValue);
    detach();
    d->m_minimumValue = minimumValue;
}

QVariant CoordinateAxis::minimumValue() const
{
    return d->m_minimumValue;
}

void CoordinateAxis::setMaximumValue(const QVariant& maximumValue) throw (BadDimensionTypeException)
{
    checkDimensionType(d->m_dimension, maximumValue);
    detach();
    d->m_maximumValue = maximumValue;
}

QVariant CoordinateAxis::maximumValue() const
{
    return d->m_maximumValue;
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

void CoordinateAxis::detach()
{
    qAtomicDetach(d);
}
