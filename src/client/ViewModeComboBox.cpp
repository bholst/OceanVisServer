//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Self
#include "ViewModeComboBox.h"

ViewModeComboBox::ViewModeComboBox(QWidget *parent)
    : QComboBox(parent)
{
    addItem("TimeLon");
    addItem("TimeLat");
    addItem("TimeHeight");
    addItem("LonLat");
    addItem("LonHeight");
    addItem("LatHeight");
    
    connect(this, SIGNAL(currentIndexChanged(int)),
            this, SLOT(emitCurrentViewModeChanged(int)));
}

ViewModeComboBox::~ViewModeComboBox()
{
}

void ViewModeComboBox::setViewMode(MainWindow::ViewMode viewMode)
{
    int index = (int) viewMode;

    if(index == currentIndex()) {
        return;
    }

    if(index >= 0) {
        setCurrentIndex(index);
    }
}

void ViewModeComboBox::emitCurrentViewModeChanged(int index)
{
    emit currentViewModeChanged((MainWindow::ViewMode) index);
}

#include "ViewModeComboBox.moc"