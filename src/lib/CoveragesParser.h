//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef COVERAGESPARSER_H
#define COVERAGESPARSER_H

// Qt
#include <QtCore/QList>

// Project
#include "XmlStreamReader.h"
#include "Constant.h"
#include "CoordinateAxis.h"
#include "Coverage.h"

class QVariant;

class CoveragesParser : public XmlStreamReader
{
public:
    CoveragesParser();
    ~CoveragesParser();
    
    void read();
    
    QList<Coverage> coverages() const;
    
private:
    void readCoverages();
    Coverage readCoverage();
    Constant readConstant();
    CoordinateAxis readCoordinateAxis();
    QList<QVariant> readValues(Dimension dimension);
    QVariant readValue(Dimension dimension);
    
    QList<Coverage> m_coverages;
};

#endif
