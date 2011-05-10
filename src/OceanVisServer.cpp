//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#include "OceanVisServer.h"

#include <QTimer>
#include <iostream>

OceanVisServer::OceanVisServer()
{
    QTimer* timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()), SLOT(output()) );
    timer->start( 1000 );
}

OceanVisServer::~OceanVisServer()
{}

void OceanVisServer::output()
{
    std::cout << "Hello World!" << std::endl;
}

#include "OceanVisServer.moc"
