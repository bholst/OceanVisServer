//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QList>
#include <QtCore/QDateTime>
#include <QCoreApplication>

// Project
#include "OceanVisServer.h"
#include "MapGeometryParser.h"
#include "MapGeometry.h"
#include "DataLayer.h"


int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    if(argc >= 3) {
        QFile file(argv[1]);
        MapGeometry geometry;
        if(file.exists()) {
            MapGeometryParser parser;
            parser.setFile(&file);
            geometry = *parser.mapGeometry();
            qDebug() << "Loaded geometry.";
            qDebug() << "Value count:" << geometry.totalSize();
        }
        else {
            qDebug() << "File does not exist.";
            return -1;
        }
        QList<DataLayer *> layers;
        for(int i = 2; i < argc; ++i) {
            DataLayer * layer = new DataLayer();
            layer->setGeometry(geometry);
            layer->setFileName(QDateTime(), argv[i]);
            layers.append(layer);
        }
        
        foreach(DataLayer *layer, layers) {
            delete layer;
        }
    }
    OceanVisServer foo;    
    return app.exec();
}
