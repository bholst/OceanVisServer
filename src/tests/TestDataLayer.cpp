//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtTest/QtTest>

// Project
#include "DimensionTrim.h"
#include "DimensionSlice.h"
#include "CoordinateAxis.h"
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
    
    void testCalculateLatLimitsTrim_data();
    void testCalculateLatLimitsTrim();
    
    void testCalculateLatLimitsSlice_data();
    void testCalculateLatLimitsSlice();
    
    void testCalculateLatLimitsNull();

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
    QTest::addColumn<double>("lowerLimitContains");
    QTest::addColumn<double>("upperLimitContains");
    QTest::addColumn<int>("lowLonTrimOverlaps");
    QTest::addColumn<int>("highLonTrimOverlaps");
    QTest::addColumn<double>("lowerLimitOverlaps");
    QTest::addColumn<double>("upperLimitOverlaps");

    QTest::newRow("full") << -180.0 << 180.0 << 0 << 4 << -180.0 << 180.0 << 0 << 4 << -180.0 << 180.0;
    QTest::newRow("nearlyFull") << -179.0 << 179.0 << 1 << 3 << -90.0 << 90.0 << 0 << 4 << -180.0 << 180.0;
    QTest::newRow("moreThanFull") << -181.0 << 181.0 << 0 << 4 << -180.0 << 180.0 << -1 << 5 << -270.0 << 270.0;
    QTest::newRow("leftHalf") << -180.0 << 0.0 << 0 << 2 << -180.0 << 0.0 << 0 << 2 << -180.0 << 0.0;
    QTest::newRow("overLeftHalf") << -180.0 << 1.0 << 0 << 2 << -180.0 << 0.0 << 0 << 3 << -180.0 << 90.0;
    QTest::newRow("rightHalf") << 0.0 << 180.0 << 2 << 4 << 0.0 << 180.0 << 2 << 4 << 0.0 << 180.0;
    QTest::newRow("overRightHalf") << -1.0 << 180.0 << 2 << 4 << 0.0 << 180.0 << 1 << 4 << -90.0 << 180.0;
}

void TestDataLayer::testCalculateLonLimitsTrim()
{
    QFETCH(double, trimLow);
    QFETCH(double, trimHigh);
    QFETCH(int, lowLonTrimContains);
    QFETCH(int, highLonTrimContains);
    QFETCH(double, lowerLimitContains);
    QFETCH(double, upperLimitContains);
    QFETCH(int, lowLonTrimOverlaps);
    QFETCH(int, highLonTrimOverlaps);
    QFETCH(double, lowerLimitOverlaps);
    QFETCH(double, upperLimitOverlaps);
    
    DimensionTrim trim(Lon);
    trim.setTrimLow(trimLow);
    trim.setTrimHigh(trimHigh);
    
    int realLowLonTrim = -100;
    int realHighLonTrim = -100;
    QList<CoordinateAxis> axes;
    m_dataLayer.calculateLonLimits(&trim, &realLowLonTrim, &realHighLonTrim, &axes, DataLayer::Contains);
    
    QCOMPARE(realLowLonTrim, lowLonTrimContains);
    QCOMPARE(realHighLonTrim, highLonTrimContains);
    QCOMPARE(axes.size(), 1);
    QCOMPARE(axes.at(0).lowerLimit().toDouble(), lowerLimitContains);
    QCOMPARE(axes.at(0).upperLimit().toDouble(), upperLimitContains);
    
    realLowLonTrim = -100;
    realHighLonTrim = -100;
    axes.clear();
    
    m_dataLayer.calculateLonLimits(&trim, &realLowLonTrim, &realHighLonTrim, &axes, DataLayer::Overlaps);
    QCOMPARE(realLowLonTrim, lowLonTrimOverlaps);
    QCOMPARE(realHighLonTrim, highLonTrimOverlaps);
    QCOMPARE(axes.size(), 1);
    QCOMPARE(axes.at(0).lowerLimit().toDouble(), lowerLimitOverlaps);
    QCOMPARE(axes.at(0).upperLimit().toDouble(), upperLimitOverlaps);
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
    QList<CoordinateAxis> axes;
    m_dataLayer.calculateLonLimits(&slice, &realLowLonTrim, &realHighLonTrim, &axes, DataLayer::Contains);
    
    QCOMPARE(realLowLonTrim, lowLonTrim);
    QCOMPARE(realHighLonTrim, highLonTrim);
    QCOMPARE(axes.size(), 0);
    
    realLowLonTrim = -100;
    realHighLonTrim = -100;
    
    m_dataLayer.calculateLonLimits(&slice, &realLowLonTrim, &realHighLonTrim, &axes, DataLayer::Overlaps);
    QCOMPARE(realLowLonTrim, lowLonTrim);
    QCOMPARE(realHighLonTrim, highLonTrim);
    QCOMPARE(axes.size(), 0);
}

void TestDataLayer::testCalculateLonLimitsNull()
{
    int realLowLonTrim = -100;
    int realHighLonTrim = -100;
    QList<CoordinateAxis> axes;
    
    m_dataLayer.calculateLonLimits(0, &realLowLonTrim, &realHighLonTrim, &axes, DataLayer::Contains);
    QCOMPARE(realLowLonTrim, 0);
    QCOMPARE(realHighLonTrim, 4);
    QCOMPARE(axes.size(), 1);
    QCOMPARE(axes.at(0).lowerLimit().toDouble(), -180.0);
    QCOMPARE(axes.at(0).upperLimit().toDouble(), 180.0);
    realLowLonTrim = -100;
    realHighLonTrim = -100;
    
    m_dataLayer.calculateLonLimits(0, &realLowLonTrim, &realHighLonTrim, &axes, DataLayer::Overlaps);
    QCOMPARE(realLowLonTrim, 0);
    QCOMPARE(realHighLonTrim, 4);
    QCOMPARE(axes.size(), 2);
    QCOMPARE(axes.at(1).lowerLimit().toDouble(), -180.0);
    QCOMPARE(axes.at(1).upperLimit().toDouble(), 180.0);
}

void TestDataLayer::testCalculateLatLimitsTrim_data()
{
    QTest::addColumn<double>("trimLow");
    QTest::addColumn<double>("trimHigh");
    QTest::addColumn<int>("lowLatTrimContains");
    QTest::addColumn<int>("highLatTrimContains");
    QTest::addColumn<double>("lowerLimitContains");
    QTest::addColumn<double>("upperLimitContains");
    QTest::addColumn<int>("lowLatTrimOverlaps");
    QTest::addColumn<int>("highLatTrimOverlaps");
    QTest::addColumn<double>("lowerLimitOverlaps");
    QTest::addColumn<double>("upperLimitOverlaps");

    QTest::newRow("full") << -90.0 << 90.0 << 0 << 2 << -90.0 << 90.0 << 0 << 2 << -90.0 << 90.0;
    QTest::newRow("nearlyFull") << -89.0 << 89.0 << 1 << 1 << 0.0 << 0.0 << 0 << 2 << -90.0 << 90.0;
    QTest::newRow("moreThanFull") << -91.0 << 91.0 << 0 << 2 << -90.0 << 90.0 << -1 << 3 << -180.0 << 180.0;
    QTest::newRow("lowerHalf") << -90.0 << 0.0 << 0 << 1 << -90.0 << 0.0 << 0 << 1 << -90.0 << 0.0;
    QTest::newRow("overLowerHalf") << -90.0 << 1.0 << 0 << 1 << -90.0 << 0.0 << 0 << 2 << -90.0 << 90.0;
    QTest::newRow("upperHalf") << 0.0 << 90.0 << 1 << 2 << 0.0 << 90.0 << 1 << 2 << 0.0 << 90.0;
    QTest::newRow("overUpperHalf") << -1.0 << 90.0 << 1 << 2 << 0.0 << 90.0 << 0 << 2 << -90.0 << 90.0;
}

void TestDataLayer::testCalculateLatLimitsTrim()
{
    QFETCH(double, trimLow);
    QFETCH(double, trimHigh);
    QFETCH(int, lowLatTrimContains);
    QFETCH(int, highLatTrimContains);
    QFETCH(double, lowerLimitContains);
    QFETCH(double, upperLimitContains);
    QFETCH(int, lowLatTrimOverlaps);
    QFETCH(int, highLatTrimOverlaps);
    QFETCH(double, lowerLimitOverlaps);
    QFETCH(double, upperLimitOverlaps);
    
    DimensionTrim trim(Lat);
    trim.setTrimLow(trimLow);
    trim.setTrimHigh(trimHigh);
    
    int realLowLatTrim = -100;
    int realHighLatTrim = -100;
    QList<CoordinateAxis> axes;
    m_dataLayer.calculateLatLimits(&trim, &realLowLatTrim, &realHighLatTrim, &axes, DataLayer::Contains);
    
    QCOMPARE(realLowLatTrim, lowLatTrimContains);
    QCOMPARE(realHighLatTrim, highLatTrimContains);
    QCOMPARE(axes.size(), 1);
    QCOMPARE(axes.at(0).lowerLimit().toDouble(), lowerLimitContains);
    QCOMPARE(axes.at(0).upperLimit().toDouble(), upperLimitContains);
    
    realLowLatTrim = -100;
    realHighLatTrim = -100;
    axes.clear();
    
    m_dataLayer.calculateLatLimits(&trim, &realLowLatTrim, &realHighLatTrim, &axes, DataLayer::Overlaps);
    QCOMPARE(realLowLatTrim, lowLatTrimOverlaps);
    QCOMPARE(realHighLatTrim, highLatTrimOverlaps);
    QCOMPARE(axes.size(), 1);
    QCOMPARE(axes.at(0).lowerLimit().toDouble(), lowerLimitOverlaps);
    QCOMPARE(axes.at(0).upperLimit().toDouble(), upperLimitOverlaps);
}

void TestDataLayer::testCalculateLatLimitsSlice_data()
{
    QTest::addColumn<double>("slicePoint");
    QTest::addColumn<int>("lowLatTrim");
    QTest::addColumn<int>("highLatTrim");

    QTest::newRow("middle") << 0.0 << 1 << 2;
    QTest::newRow("left") << -90.0 << 0 << 1;
    QTest::newRow("right") << 90.0 << 2 << 3;
    QTest::newRow("centerLeft") << -45.0 << 0 << 1;
}

void TestDataLayer::testCalculateLatLimitsSlice()
{
    QFETCH(double, slicePoint);
    QFETCH(int, lowLatTrim);
    QFETCH(int, highLatTrim);
    
    DimensionSlice slice(Lat);
    slice.setSlicePoint(slicePoint);
    
    int realLowLatTrim = -100;
    int realHighLatTrim = -100;
    QList<CoordinateAxis> axes;
    m_dataLayer.calculateLatLimits(&slice, &realLowLatTrim, &realHighLatTrim, &axes, DataLayer::Contains);
    
    QCOMPARE(realLowLatTrim, lowLatTrim);
    QCOMPARE(realHighLatTrim, highLatTrim);
    QCOMPARE(axes.size(), 0);
    
    realLowLatTrim = -100;
    realHighLatTrim = -100;
    
    m_dataLayer.calculateLatLimits(&slice, &realLowLatTrim, &realHighLatTrim, &axes, DataLayer::Overlaps);
    QCOMPARE(realLowLatTrim, lowLatTrim);
    QCOMPARE(realHighLatTrim, highLatTrim);
    QCOMPARE(axes.size(), 0);
}

void TestDataLayer::testCalculateLatLimitsNull()
{
    int realLowLatTrim = -100;
    int realHighLatTrim = -100;
    QList<CoordinateAxis> axes;
    
    m_dataLayer.calculateLatLimits(0, &realLowLatTrim, &realHighLatTrim, &axes, DataLayer::Contains);
    QCOMPARE(realLowLatTrim, 0);
    QCOMPARE(realHighLatTrim, 2);
    QCOMPARE(axes.size(), 1);
    QCOMPARE(axes.at(0).lowerLimit().toDouble(), -90.0);
    QCOMPARE(axes.at(0).upperLimit().toDouble(), 90.0);
    realLowLatTrim = -100;
    realHighLatTrim = -100;
    
    m_dataLayer.calculateLatLimits(0, &realLowLatTrim, &realHighLatTrim, &axes, DataLayer::Overlaps);
    QCOMPARE(realLowLatTrim, 0);
    QCOMPARE(realHighLatTrim, 2);
    QCOMPARE(axes.size(), 2);
    QCOMPARE(axes.at(1).lowerLimit().toDouble(), -90.0);
    QCOMPARE(axes.at(1).upperLimit().toDouble(), 90.0);
}

QTEST_MAIN( TestDataLayer )
#include "TestDataLayer.moc"
