//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef GLOBAL_H
#define GLOBAL_H

#include<QtGlobal>

enum Dimension {
    Time,
    Lon,
    Lat,
    Height
};

const qreal MINLON = -180.0;
const qreal MAXLON = 180.0;
const qreal DELTALON = 360.0;

const qreal MINLAT = -90.0;
const qreal MAXLAT = 90.0;
const qreal DELTALAT = 180.0;

#endif
