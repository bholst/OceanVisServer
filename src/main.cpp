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
#include <QtGui/QImage>
#include <QCoreApplication>

// Project
#include "DataLayer.h"
#include "OceanVisServer.h"
#include "ConfigurationParser.h"


int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    if(argc >= 2) {
        QString path(argv[1]);
        ConfigurationParser parser;
        parser.setPath(path);
        parser.read();
        QList<DataLayer *> layers = parser.layers();

        foreach(DataLayer *layer, layers) {
            delete layer;
        }
    }

    OceanVisServer foo;
    foo.listen(QHostAddress::Any, 8080);
    return app.exec();
}
