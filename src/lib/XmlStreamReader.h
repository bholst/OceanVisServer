//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef XMLSTREAMREADER_H
#define XMLSTREAMREADER_H

#include <QtCore/QXmlStreamReader>

class XmlStreamReader : public QXmlStreamReader {
public:
    XmlStreamReader();
    ~XmlStreamReader();
    
    void readUnknownElement();
    QString readCharacters();
};

#endif
