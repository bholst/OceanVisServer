//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <iostream>

// Qt
#include <QtCore/QDebug>
#include <QApplication>

// Project
#include "MainWindow.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setApplicationName("Ocean Visualization");
    
    MainWindow window;
    window.setUrl("http://127.0.0.1:8080/ovp");
    window.show();
    
    return app.exec();
}
