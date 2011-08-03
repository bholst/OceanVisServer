//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef COVERAGECOMBOBOX_H
#define COVERAGECOMBOBOX_H

// Project
#include "Coverage.h"

// Qt
#include <QtCore/QList>
#include <QtGui/QComboBox>


class CoverageComboBox : public QComboBox {
    Q_OBJECT
public:
    CoverageComboBox(QWidget *parent = 0);
    ~CoverageComboBox();
    
public slots:
    void setCoverages(const QList<Coverage>& coverages);
    void setCoverageId(const QString& coverageId);
    
signals:
    void currentCoverageChanged(const QString& coverageId);
    
private:
};

#endif
