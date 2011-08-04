//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Self
#include "XmlStreamReader.h"

XmlStreamReader::XmlStreamReader()
{
}

XmlStreamReader::~XmlStreamReader()
{
}

void XmlStreamReader::readUnknownElement()
{
    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement())
            readUnknownElement();
    }
}

QString XmlStreamReader::readCharacters()
{
    Q_ASSERT(isStartElement());

    QString string;

    while(!atEnd()) {
        readNext();

        if(isEndElement())
            break;

        if(isStartElement()) {
            readUnknownElement();
        }

        if(isCharacters()) {
            string = text().toString();
        }
    }

    return string;
}
