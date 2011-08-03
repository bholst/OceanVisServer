//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtCore/QDebug>

// Self
#include "ParseTime.h"

QDateTime parseTime(QString timeString)
{
    QDateTime time;
    bool success = false;
    uint seconds = timeString.toUInt(&success);
    if(success) {
        qDebug() << "Found seconds.";
        time.setTime_t(seconds);
    }
    else {
        qDebug() << "ISO time.";
        time = QDateTime::fromString(timeString, Qt::ISODate);
    }
    
    return time;
}

