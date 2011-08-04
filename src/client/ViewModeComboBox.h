//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef VIEWMODECOMBOBOX_H
#define VIEWMODECOMBOBOX_H

// Project
#include "MainWindow.h"

// Qt
#include <QtGui/QComboBox>


class ViewModeComboBox : public QComboBox {
    Q_OBJECT
public:
    ViewModeComboBox(QWidget *parent = 0);
    ~ViewModeComboBox();
    
public slots:
    void setViewMode(MainWindow::ViewMode viewMode);
    
signals:
    void currentViewModeChanged(MainWindow::ViewMode viewMode);
    
private slots:
    void emitCurrentViewModeChanged(int index);
};

#endif
