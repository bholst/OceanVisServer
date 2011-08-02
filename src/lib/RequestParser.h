//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef REQUESTPARSER_H
#define REQUESTPARSER_H

// Qt
#include <QtCore/QXmlStreamReader>

class RequestBase;
class DimensionTrim;
class DimensionSlice;
class QDateTime;

class RequestParser : public QXmlStreamReader
{
public:
    RequestParser();
    ~RequestParser();
    
    void read();
    
    RequestBase *request() const;
    
private:
    void readGetCoverage();
    DimensionTrim *readDimensionTrim();
    DimensionSlice *readDimensionSlice();
    void readUnknownElement();
    QString readCharacters();
    
    RequestBase *m_request;
};

#endif
