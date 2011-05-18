//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>
#include <QtCore/QVariant>

// Project
#include "DimensionSubset_p.h"

// Self
#include "DimensionSlice.h"

class DimensionSlicePrivate : public DimensionSubsetPrivate {
public:
    DimensionSlicePrivate()
        : DimensionSubsetPrivate(),
          m_slicePoint(0.0)
    {
    }

    DimensionSlicePrivate(const DimensionSlicePrivate& other)
        : DimensionSubsetPrivate(other),
          m_slicePoint(other.m_slicePoint)
    {
    }

    ~DimensionSlicePrivate()
    {
    }

    DimensionSlicePrivate& operator=(const DimensionSlicePrivate &other)
    {
        DimensionSubsetPrivate::operator=(other);
        m_slicePoint = other.m_slicePoint;
        return *this;
    }

    bool operator==(const DimensionSlicePrivate &other)
    {
        return DimensionSubsetPrivate::operator==(other)
               && m_slicePoint == other.m_slicePoint;
    }

    QVariant m_slicePoint;
};

DimensionSlice::DimensionSlice()
    : DimensionSubset(new DimensionSlicePrivate())
{
}

DimensionSlice::DimensionSlice(const DimensionSlice &other)
    : DimensionSubset(other)
{
}

DimensionSlice::~DimensionSlice()
{
}

void DimensionSlice::setSlicePoint(QVariant slicePoint)
{
    detach();
    p()->m_slicePoint = slicePoint;
}

QVariant DimensionSlice::slicePoint() const
{
    return p()->m_slicePoint;
}

DimensionSlice& DimensionSlice::operator=(const DimensionSlice &other)
{
    return (DimensionSlice &) DimensionSubset::operator=(other);
}

bool DimensionSlice::operator==(const DimensionSlice &other) const
{
    return p() == other.p();
}

void DimensionSlice::detach()
{
    qAtomicDetach((DimensionSlicePrivate *&) d);
}

DimensionSlicePrivate *DimensionSlice::p() const
{
    return (DimensionSlicePrivate *) d;
}
