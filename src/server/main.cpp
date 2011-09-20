//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <iostream>

// Qt
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QDateTime>
#include <QtGui/QImage>
#include <QApplication>

// Project
#include "DataLayer.h"
#include "OceanVisServer.h"
#include "ConfigurationParser.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QHash<QString,DataLayer *> layers;

    if(argc >= 2) {
        QString path(argv[1]);
        ConfigurationParser parser;
        parser.setPath(path);
        parser.read();
        layers = parser.layers();
    }

    OceanVisServer server;
    server.setLayers(layers);
    server.listen(QHostAddress::Any, 8080);
    return app.exec();
}
