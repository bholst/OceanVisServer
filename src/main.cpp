//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#include <QCoreApplication>
#include "OceanVisServer.h"


int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    OceanVisServer foo;
    return app.exec();
}
