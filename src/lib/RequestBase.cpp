//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QString>

// Self
#include "RequestBase.h"

RequestBase::RequestBase(RequestType requestType)
    : m_requestType(requestType)
{
}

RequestBase::~RequestBase()
{
}

RequestBase::RequestType RequestBase::requestType() const
{
    return m_requestType;
}

QString RequestBase::version() const
{
    return m_version;
}

void RequestBase::setVersion(const QString& version)
{
    m_version = version;
}
