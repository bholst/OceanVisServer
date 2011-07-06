//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#include <QtCore/QObject>
#include <QtTest/QtTest>

// Project
#include "DimensionTrim.h"
#include "DimensionSlice.h"
#include "MapGeometry.h"
#include "DataLayer.h"

class TestDataLayer : public QObject {
    Q_OBJECT
public:
    TestDataLayer();
    ~TestDataLayer();

private Q_SLOTS:
    void initTestCase();
    void testSetGeometry();
    void testCalculateLonLimitsTrim_data();
    void testCalculateLonLimitsTrim();
    
    void testCalculateLonLimitsSlice_data();
    void testCalculateLonLimitsSlice();
    
    void testCalculateLonLimitsNull();

private:
    MapGeometry m_geometry;
    DataLayer m_dataLayer;
};

void TestDataLayer::initTestCase()
{
    m_geometry.setWidth(4);
    m_geometry.setHeight(2);

    int layerCounts[] = {1, 2, 0, 3,
                         4, 0, 5, 6};
    m_geometry.setLayerCounts(layerCounts);
    
    m_dataLayer.setGeometry(m_geometry);
}

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

void TestDataLayer::testCalculateLonLimitsTrim_data()
{
    QTest::addColumn<double>("trimLow");
    QTest::addColumn<double>("trimHigh");
    QTest::addColumn<int>("lowLonTrimContains");
    QTest::addColumn<int>("highLonTrimContains");
    QTest::addColumn<int>("lowLonTrimOverlaps");
    QTest::addColumn<int>("highLonTrimOverlaps");

    QTest::newRow("full") << -180.0 << 180.0 << 0 << 4 << 0 << 4;
    QTest::newRow("nearlyFull") << -179.0 << 179.0 << 1 << 3 << 0 << 4;
    QTest::newRow("moreThanFull") << -181.0 << 181.0 << 0 << 4 << -1 << 5;
    QTest::newRow("leftHalf") << -180.0 << 0.0 << 0 << 2 << 0 << 2;
    QTest::newRow("overLeftHalf") << -180.0 << 1.0 << 0 << 2 << 0 << 3;
    QTest::newRow("rightHalf") << 0.0 << 180.0 << 2 << 4 << 2 << 4;
    QTest::newRow("overRightHalf") << -1.0 << 180.0 << 2 << 4 << 1 << 4;
}

void TestDataLayer::testCalculateLonLimitsTrim()
{
    QFETCH(double, trimLow);
    QFETCH(double, trimHigh);
    QFETCH(int, lowLonTrimContains);
    QFETCH(int, highLonTrimContains);
    QFETCH(int, lowLonTrimOverlaps);
    QFETCH(int, highLonTrimOverlaps);
    
    DimensionTrim trim(Lon);
    trim.setTrimLow(trimLow);
    trim.setTrimHigh(trimHigh);
    
    int realLowLonTrim = -100;
    int realHighLonTrim = -100;
    m_dataLayer.calculateLonLimits(&trim, &realLowLonTrim, &realHighLonTrim, DataLayer::Contains);
    
    QCOMPARE(realLowLonTrim, lowLonTrimContains);
    QCOMPARE(realHighLonTrim, highLonTrimContains);
    
    realLowLonTrim = -100;
    realHighLonTrim = -100;
    
    m_dataLayer.calculateLonLimits(&trim, &realLowLonTrim, &realHighLonTrim, DataLayer::Overlaps);
    QCOMPARE(realLowLonTrim, lowLonTrimOverlaps);
    QCOMPARE(realHighLonTrim, highLonTrimOverlaps);
}

void TestDataLayer::testCalculateLonLimitsSlice_data()
{
    QTest::addColumn<double>("slicePoint");
    QTest::addColumn<int>("lowLonTrim");
    QTest::addColumn<int>("highLonTrim");

    QTest::newRow("middle") << 0.0 << 2 << 3;
    QTest::newRow("left") << -180.0 << 0 << 1;
    QTest::newRow("right") << 180.0 << 4 << 5;
    QTest::newRow("centerLeft") << -45.0 << 1 << 2;
}

void TestDataLayer::testCalculateLonLimitsSlice()
{
    QFETCH(double, slicePoint);
    QFETCH(int, lowLonTrim);
    QFETCH(int, highLonTrim);
    
    DimensionSlice slice(Lon);
    slice.setSlicePoint(slicePoint);
    
    int realLowLonTrim = -100;
    int realHighLonTrim = -100;
    m_dataLayer.calculateLonLimits(&slice, &realLowLonTrim, &realHighLonTrim, DataLayer::Contains);
    
    QCOMPARE(realLowLonTrim, lowLonTrim);
    QCOMPARE(realHighLonTrim, highLonTrim);
    
    realLowLonTrim = -100;
    realHighLonTrim = -100;
    
    m_dataLayer.calculateLonLimits(&slice, &realLowLonTrim, &realHighLonTrim, DataLayer::Overlaps);
    QCOMPARE(realLowLonTrim, lowLonTrim);
    QCOMPARE(realHighLonTrim, highLonTrim);
}

void TestDataLayer::testCalculateLonLimitsNull()
{
    int realLowLonTrim = -100;
    int realHighLonTrim = -100;
    
    m_dataLayer.calculateLonLimits(0, &realLowLonTrim, &realHighLonTrim, DataLayer::Contains);
    QCOMPARE(realLowLonTrim, 0);
    QCOMPARE(realHighLonTrim, 4);
    realLowLonTrim = -100;
    realHighLonTrim = -100;
    
    m_dataLayer.calculateLonLimits(0, &realLowLonTrim, &realHighLonTrim, DataLayer::Overlaps);
    QCOMPARE(realLowLonTrim, 0);
    QCOMPARE(realHighLonTrim, 4);
}

QTEST_MAIN( TestDataLayer )
#include "TestDataLayer.moc"
