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
#include "Constant.h"

class ConstantPrivate {
public:
    ConstantPrivate(Dimension dimension)
        : m_dimension(dimension),
          ref(1)
    {
    }

    ConstantPrivate(const ConstantPrivate& other)
        : m_dimension(other.m_dimension),
          m_value(other.m_value),
          ref(1)
    {
    }

    ~ConstantPrivate()
    {
    }

    /*
     * Return this instead of other.
     */
    ConstantPrivate& operator=(const ConstantPrivate &other)
    {
        m_dimension = other.m_dimension;
        m_value = other.m_value;
        ref = other.ref;
        return *this;
    }

    bool operator==(const ConstantPrivate &other)
    {
        return m_dimension == other.m_dimension
               && m_value == other.m_value;
    }

    Dimension m_dimension;
    QVariant m_value;
    QAtomicInt ref;
};

Constant::Constant(Dimension dimension)
    : d(new ConstantPrivate(dimension))
{
}

Constant::Constant(const Constant &other)
{
    this->d = other.d;
    this->d->ref.ref();
}

Constant::~Constant()
{
    if(!d->ref.deref())
        delete d;
}

Dimension Constant::dimension() const
{
    return d->m_dimension;
}

void Constant::setValue(const QVariant& value) throw (BadDimensionTypeException)
{
    checkDimensionType(d->m_dimension, value);
    detach();
    d->m_value = value;
}

QVariant Constant::value() const
{
    return d->m_value;
}

Constant& Constant::operator=(const Constant &other)
{
    qAtomicAssign(d, other.d);
    return *this;
}

bool Constant::operator==(const Constant &other)
{
    return d == other.d || (*d) == (*other.d);
}

void Constant::detach()
{
    qAtomicDetach(d);
}
