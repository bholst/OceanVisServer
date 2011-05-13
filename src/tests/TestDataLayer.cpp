//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "MapGeometry.h"
#include "DataLayer.h"

class TestDataLayer : public QObject {
    Q_OBJECT
public:
    TestDataLayer();
    ~TestDataLayer();

private Q_SLOTS:
    void testSetGeometry();

private:
};

TestDataLayer::TestDataLayer()
{
}

TestDataLayer::~TestDataLayer()
{
}

void TestDataLayer::testSetGeometry()
{
    MapGeometry geometry;
    geometry.setWidth(2);
    
    DataLayer layer;
    layer.setGeometry(geometry);
    
    QCOMPARE(layer.geometry().width(), 2);
}

QTEST_MAIN( TestDataLayer )
#include "TestDataLayer.moc"
