//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Self
#include "DimensionSubset.h"
#include "DimensionSubset_p.h"

DimensionSubsetPrivate::DimensionSubsetPrivate() 
    : m_dimension(DimensionSubset::Time),
      ref(1)
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

DimensionSubset::DimensionSubset()
{
    d = new DimensionSubsetPrivate();
}

DimensionSubset::DimensionSubset(const DimensionSubset &other)
{
    d = other.d;
    d->ref.ref();
}

DimensionSubset::DimensionSubset(DimensionSubsetPrivate *d)
{
    this->d = d;
    d->ref.ref();
}
    
DimensionSubset::~DimensionSubset() 
{
    if (!d->ref.deref())
        delete d;
}

void DimensionSubset::setDimension(DimensionSubset::Dimension dimension)
{
    detach();
    p()->m_dimension = dimension;
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
