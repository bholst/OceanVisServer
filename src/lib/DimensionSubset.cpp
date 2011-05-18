//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>

// Self
#include "DimensionSubset.h"
#include "DimensionSubset_p.h"

DimensionSubsetPrivate::DimensionSubsetPrivate(DimensionSubset::Dimension dimension) 
    : m_dimension(dimension),
      ref(1)
{
}

DimensionSubsetPrivate::DimensionSubsetPrivate(const DimensionSubsetPrivate& other)
    : m_dimension(other.m_dimension),
      ref(other.ref)
{
}

DimensionSubsetPrivate::~DimensionSubsetPrivate()
{
}

/*
 * Return this instead of other.
 */
DimensionSubsetPrivate& DimensionSubsetPrivate::operator=(const DimensionSubsetPrivate &other)
{
    m_dimension = other.m_dimension;
    ref = other.ref;
    return *this;
}

bool DimensionSubsetPrivate::operator==(const DimensionSubsetPrivate &other)
{
    return m_dimension == other.m_dimension;
}

DimensionSubset::DimensionSubset(DimensionSubset::Dimension dimension)
{
    d = new DimensionSubsetPrivate(dimension);
}

DimensionSubset::DimensionSubset(const DimensionSubset &other)
{
    this->d = other.d;
    this->d->ref.ref();
}

DimensionSubset::DimensionSubset(DimensionSubsetPrivate *d)
{
    this->d = d;
}

DimensionSubset::~DimensionSubset() 
{
    if (!d->ref.deref())
        delete d;
}

DimensionSubset::Dimension DimensionSubset::dimension() const
{
    return p()->m_dimension;
}

DimensionSubset& DimensionSubset::operator=(const DimensionSubset &other)
{
    qAtomicAssign(d, other.d);
    return *this;
}

bool DimensionSubset::operator==(const DimensionSubset &other) const
{
    return p() == other.p();
}

void DimensionSubset::detach()
{
    qAtomicDetach(d);
}

DimensionSubsetPrivate *DimensionSubset::p() const
{
    return d;
}
