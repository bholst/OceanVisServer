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

        DimensionTrim lon(Lon);
        lon.setTrimLow(-10.0);
        lon.setTrimHigh(0.0);
//         subsets.append(&lon);

        DimensionSlice lat(Lat);
        lat.setSlicePoint(-90.0);
        subsets.append(&lat);

        DimensionSlice height(Height);
        height.setSlicePoint(0.0);
//         subsets.append(&height);

        QList<DataLayer *> layers;
        for(int i = 2; i < argc; ++i) {
            DataLayer * layer = new DataLayer();
            layer->setGeometry(geometry);
            layer->setFileName(QDateTime::currentDateTime(), argv[i]);
            layers.append(layer);

            DataMatrix *matrix = layer->dataSubset(subsets);
            qDebug() << matrix->toString();
        }

        foreach(DataLayer *layer, layers) {
            delete layer;
        }
    }
    OceanVisServer foo;
    return app.exec();
}
