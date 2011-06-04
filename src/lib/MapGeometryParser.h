//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef MAPGEOMETRYPARSER_H
#define MAPGEOMETRYPARSER_H

class QFile;
class MapGeometry;

class MapGeometryParser {
public:
    MapGeometryParser();
    ~MapGeometryParser();
    
    /**
     * Set the file to be parsed.
     * MapGeometryParser will not take ownership of the QFile.
     */
    void setFile(QFile *file);
    void setTextStream(QTextStream *textStream);
    
    QTextStream *textStream() const;
    
    MapGeometry mapGeometry() const;
    
private:
    void parse();
    
    QTextStream *m_textStream;
};

#endif
