//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QCoreApplication>

// Project
#include "OceanVisServer.h"
#include "MapGeometryParser.h"


int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    for(int i = 1; i < argc; ++i) {
        QFile file(argv[i]);
        if(file.exists()) {
            MapGeometryParser parser;
            parser.setFile(&file);
            MapGeometry *geometry = parser.mapGeometry();
            qDebug() << "Loaded geometry.";
        }
        else {
            qDebug() << "File does not exist.";
        }
    }
    OceanVisServer foo;    
    return app.exec();
}
