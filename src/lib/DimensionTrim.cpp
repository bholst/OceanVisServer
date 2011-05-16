//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Project
#include "DimensionSubset_p.h"

// Self
#include "DimensionTrim.h"

class DimensionTrimPrivate : public DimensionSubsetPrivate {
public:
    DimensionTrimPrivate()
        : DimensionSubsetPrivate(),
          m_trimLow(0.0),
          m_trimHigh(0.0)
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

    double m_trimLow;
    double m_trimHigh;
};

DimensionTrim::DimensionTrim()
    : DimensionSubset(new DimensionTrimPrivate())
{
}

DimensionTrim::DimensionTrim(const DimensionTrim &other)
    : DimensionSubset(other)
{
}

DimensionTrim::~DimensionTrim()
{
}

void DimensionTrim::setTrimLow(double trimLow)
{
    detach();
    p()->m_trimLow = trimLow;
}

double DimensionTrim::trimLow() const
{
    return p()->m_trimLow;
}

void DimensionTrim::setTrimHigh(double trimHigh)
{
    detach();
    p()->m_trimHigh = trimHigh;
}

double DimensionTrim::trimHigh() const
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