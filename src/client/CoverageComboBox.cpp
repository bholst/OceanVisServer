//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#include "CoverageComboBox.h"

CoverageComboBox::CoverageComboBox(QWidget *parent)
    : QComboBox(parent)
{
    connect(this, SIGNAL(currentIndexChanged(const QString &)),
            this, SIGNAL(currentCoverageChanged(const QString&)));
}

CoverageComboBox::~CoverageComboBox()
{
}

void CoverageComboBox::setCoverageId(const QString& coverageId)
{
    if(coverageId == currentText()) {
        return;
    }

    int index = findText(coverageId);
    if(index >= 0) {
        setCurrentIndex(index);
    }
    else {
        setCurrentIndex(0);
    }
}
    
void CoverageComboBox::setCoverages(const QList<Coverage>& coverages)
{
    clear();
    
    foreach(Coverage coverage, coverages) {
        addItem(coverage.coverageId());
    }
}


#include "CoverageComboBox.moc"