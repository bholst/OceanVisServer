//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <iostream>

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
#include "DataMatrix.h"
#include "DimensionSlice.h"
#include "DimensionSubset.h"
#include "DimensionTrim.h"


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
            geometry.setHeightDimension(-10.0);
            qDebug() << "Loaded geometry.";
            qDebug() << "Value count:" << geometry.totalSize();
            qDebug() << "Width:" << geometry.width();
            qDebug() << "Height:" << geometry.height();
            qDebug() << "HeightDimension:" << geometry.heightDimension();
        }
        else {
            qDebug() << "File does not exist.";
            return -1;
        }

        QList<DimensionSubset*> subsets;
        DimensionSlice slice(Time);
        slice.setSlicePoint(QDateTime::currentDateTime());
        subsets.append(&slice);

        DimensionSlice lon(Lon);
//         lon.setTrimLow(-180.0);
//         lon.setTrimHigh(-23.0);
        lon.setSlicePoint(-46);
//         subsets.append(&lon);

        DimensionSlice lat(Lat);
//         lat.setTrimLow(0.0);
//         lat.setTrimHigh(90.0);
        lat.setSlicePoint(-0.1);
        subsets.append(&lat);

        DimensionSlice height(Height);
        height.setSlicePoint(0.0);
        subsets.append(&height);

        QList<DataLayer *> layers;
        for(int i = 2; i < argc; ++i) {
            DataLayer * layer = new DataLayer();
            layer->setGeometry(geometry);
            layer->setFileName(QDateTime::currentDateTime(), argv[i]);
            layers.append(layer);

            DataMatrix *matrix = layer->dataSubset(subsets);
            std::cout << matrix->toString().toStdString() << std::endl;
        }

        foreach(DataLayer *layer, layers) {
            delete layer;
        }
    }
//     OceanVisServer foo;
//     return app.exec();
}
