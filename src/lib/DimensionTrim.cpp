//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>
#include <QtCore/QVariant>

// Project
#include "DimensionSubset_p.h"

// Self
#include "DimensionTrim.h"

class DimensionTrimPrivate : public DimensionSubsetPrivate {
public:
    DimensionTrimPrivate(Dimension dimension)
        : DimensionSubsetPrivate(dimension),
          m_trimLow(0.0),
          m_trimHigh(0.0)
    {
    }
    
    DimensionTrimPrivate(QString dimension)
        : DimensionSubsetPrivate(dimension),
          m_trimLow(0.0),
          m_trimHigh(0.0)
    {
    }

    DimensionTrimPrivate(const DimensionTrimPrivate& other)
        : DimensionSubsetPrivate(other),
          m_trimLow(other.m_trimLow),
          m_trimHigh(other.m_trimHigh)
    {
    }

    ~DimensionTrimPrivate()
    {
    }

    DimensionTrimPrivate& operator=(const DimensionTrimPrivate &other)
    {
        DimensionSubsetPrivate::operator=(other);
        m_trimLow = other.m_trimLow;
        m_trimHigh = other.m_trimHigh;
        return *this;
    }

    bool operator==(const DimensionTrimPrivate &other)
    {
        return DimensionSubsetPrivate::operator==(other)
               && m_trimLow == other.m_trimLow
               && m_trimHigh == other.m_trimHigh;
    }

    QVariant m_trimLow;
    QVariant m_trimHigh;
};

DimensionTrim::DimensionTrim(Dimension dimension)
    : DimensionSubset(new DimensionTrimPrivate(dimension))
{
}

DimensionTrim::DimensionTrim(QString dimension) throw (BadDimensionString)
    : DimensionSubset(new DimensionTrimPrivate(dimension))
{
}

DimensionTrim::DimensionTrim(const DimensionTrim &other)
    : DimensionSubset(other)
{
}

DimensionTrim::~DimensionTrim()
{
}

void DimensionTrim::setTrimLow(QVariant trimLow) throw(BadDimensionTypeException)
{
    ensureDimensionType(trimLow);

    detach();
    p()->m_trimLow = trimLow;
}

QVariant DimensionTrim::trimLow() const
{
    return p()->m_trimLow;
}

void DimensionTrim::setTrimHigh(QVariant trimHigh) throw(BadDimensionTypeException)
{
    ensureDimensionType(trimHigh);

    detach();
    p()->m_trimHigh = trimHigh;
}

QVariant DimensionTrim::trimHigh() const
{
    return p()->m_trimHigh;
}

DimensionTrim& DimensionTrim::operator=(const DimensionTrim &other)
{
    return (DimensionTrim &) DimensionSubset::operator=(other);
}

bool DimensionTrim::operator==(const DimensionTrim &other) const
{
    return p() == other.p();
}

void DimensionTrim::detach()
{
    qAtomicDetach((DimensionTrimPrivate *&) d);
}

DimensionTrimPrivate *DimensionTrim::p() const
{
    return (DimensionTrimPrivate *) d;
}
