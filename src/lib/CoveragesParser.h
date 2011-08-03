//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef COVERAGESPARSER_H
#define COVERAGESPARSER_H

// Qt
#include <QtCore/QXmlStreamReader>
#include <QtCore/QList>

// Project
#include "Constant.h"
#include "CoordinateAxis.h"
#include "Coverage.h"

class CoveragesParser : public QXmlStreamReader
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
    void readUnknownElement();
    QString readCharacters();
    
    QList<Coverage> m_coverages;
};

#endif
