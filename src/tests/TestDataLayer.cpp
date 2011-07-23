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
    
    void testCalculateHeightLimitsTrim_data();
    void testCalculateHeightLimitsTrim();
    void testCalculateHeightLimitsSlice_data();
    void testCalculateHeightLimitsSlice();
    void testCalculateHeightLimitsNull_data();
    void testCalculateHeightLimitsNull();

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

void TestDataLayer::testCalculateHeightLimitsTrim_data()
{
    QTest::addColumn<double>("heightDimension");
    QTest::addColumn<double>("trimLow");
    QTest::addColumn<double>("trimHigh");
    QTest::addColumn<int>("lowHeightTrimContains");
    QTest::addColumn<int>("highHeightTrimContains");
    QTest::addColumn<double>("lowerLimitContains");
    QTest::addColumn<double>("upperLimitContains");
    QTest::addColumn<int>("lowHeightTrimOverlaps");
    QTest::addColumn<int>("highHeightTrimOverlaps");
    QTest::addColumn<double>("lowerLimitOverlaps");
    QTest::addColumn<double>("upperLimitOverlaps");

    QTest::newRow("full") << 1.0 << 0.0 << 6.0 << 0 << 6 << 0.0 << 6.0 << 0 << 6 << 0.0 << 6.0;
    QTest::newRow("fullNeg") << -1.0 << -6.0 << 0.0 << 0 << 6 << -6.0 << 0.0 << 0 << 6 << -6.0 << 0.0;
    QTest::newRow("nearlyFull") << -10.0 << -59.0 << -1.0 << 1 << 5 << -50.0 << -10.0 << 0 << 6 << -60.0 << 0.0;
    QTest::newRow("moreThanFull") << 10.0 << -1.0 << 61.0 << 0 << 6 << 0.0 << 60.0 << -1 << 7 << -10.0 << 70.0;
    QTest::newRow("higherHalf") << -2.0 << -6.0 << 0.0 << 0 << 3 << -6.0 << 0.0 << 0 << 3 << -6.0 << 0.0;
    QTest::newRow("overHigherHalf") << -2.0 << -7.0 << 0.0 << 0 << 3 << -6.0 << 0.0 << 0 << 4 << -8.0 << 0.0;
    QTest::newRow("lowerHalf") << -2.0 << -12.0 << -6.0 << 3 << 6 << -12.0 << -6.0 << 3 << 6 << -12.0 << -6.0;
    QTest::newRow("overLowerHalf") << -2.0 << -12.0 << -5.0 << 3 << 6 << -12.0 << -6.0 << 2 << 6 << -12.0 << -4.0;
}

void TestDataLayer::testCalculateHeightLimitsTrim()
{
    QFETCH(double, heightDimension);
    QFETCH(double, trimLow);
    QFETCH(double, trimHigh);
    QFETCH(int, lowHeightTrimContains);
    QFETCH(int, highHeightTrimContains);
    QFETCH(double, lowerLimitContains);
    QFETCH(double, upperLimitContains);
    QFETCH(int, lowHeightTrimOverlaps);
    QFETCH(int, highHeightTrimOverlaps);
    QFETCH(double, lowerLimitOverlaps);
    QFETCH(double, upperLimitOverlaps);
    
    m_geometry.setHeightDimension(heightDimension);
    m_dataLayer.setGeometry(m_geometry);
    
    DimensionTrim trim(Height);
    trim.setTrimLow(trimLow);
    trim.setTrimHigh(trimHigh);
    
    int realLowHeightTrim = -100;
    int realHighHeightTrim = -100;
    QList<CoordinateAxis> axes;
    m_dataLayer.calculateHeightLimits(&trim, &realLowHeightTrim, &realHighHeightTrim, &axes, DataLayer::Contains);
    
    QCOMPARE(realLowHeightTrim, lowHeightTrimContains);
    QCOMPARE(realHighHeightTrim, highHeightTrimContains);
    QCOMPARE(axes.size(), 1);
    QCOMPARE(axes.at(0).lowerLimit().toDouble(), lowerLimitContains);
    QCOMPARE(axes.at(0).upperLimit().toDouble(), upperLimitContains);
    
    realLowHeightTrim = -100;
    realHighHeightTrim = -100;
    axes.clear();
    
    m_dataLayer.calculateHeightLimits(&trim, &realLowHeightTrim, &realHighHeightTrim, &axes, DataLayer::Overlaps);
    QCOMPARE(realLowHeightTrim, lowHeightTrimOverlaps);
    QCOMPARE(realHighHeightTrim, highHeightTrimOverlaps);
    QCOMPARE(axes.size(), 1);
    QCOMPARE(axes.at(0).lowerLimit().toDouble(), lowerLimitOverlaps);
    QCOMPARE(axes.at(0).upperLimit().toDouble(), upperLimitOverlaps);
}

void TestDataLayer::testCalculateHeightLimitsSlice_data()
{
    QTest::addColumn<double>("heightDimension");
    QTest::addColumn<double>("slicePoint");
    QTest::addColumn<int>("lowHeightTrim");
    QTest::addColumn<int>("highHeightTrim");

    QTest::newRow("firstBorderPos") << 1.0 << 0.0 << 0 << 1;
    QTest::newRow("firstBorderNeg") << -1.0 << 0.0 << 0 << 1;
    QTest::newRow("first") << -10.0 << -5.0 << 0 << 1;
    QTest::newRow("last") << -10.0 << -55.0 << 5 << 6;
    QTest::newRow("lastBorderPos") << 10.0 << 60.0 << 6 << 7;
    QTest::newRow("lastBorderNeg") << -10.0 << -60.0 << 6 << 7;
}

void TestDataLayer::testCalculateHeightLimitsSlice()
{
    QFETCH(double, heightDimension);
    QFETCH(double, slicePoint);
    QFETCH(int, lowHeightTrim);
    QFETCH(int, highHeightTrim);
    
    m_geometry.setHeightDimension(heightDimension);
    m_dataLayer.setGeometry(m_geometry);
    
    DimensionSlice slice(Height);
    slice.setSlicePoint(slicePoint);
    
    int realLowHeightTrim = -100;
    int realHighHeightTrim = -100;
    QList<CoordinateAxis> axes;
    m_dataLayer.calculateHeightLimits(&slice, &realLowHeightTrim, &realHighHeightTrim, &axes, DataLayer::Contains);
    
    QCOMPARE(realLowHeightTrim, lowHeightTrim);
    QCOMPARE(realHighHeightTrim, highHeightTrim);
    QCOMPARE(axes.size(), 0);
    
    realLowHeightTrim = -100;
    realHighHeightTrim = -100;
    
    m_dataLayer.calculateHeightLimits(&slice, &realLowHeightTrim, &realHighHeightTrim, &axes, DataLayer::Overlaps);
    QCOMPARE(realLowHeightTrim, lowHeightTrim);
    QCOMPARE(realHighHeightTrim, highHeightTrim);
    QCOMPARE(axes.size(), 0);
}

void TestDataLayer::testCalculateHeightLimitsNull_data()
{
    QTest::addColumn<double>("heightDimension");
    QTest::addColumn<double>("lowerLimit");
    QTest::addColumn<double>("upperLimit");
    
    QTest::newRow("pos1") << 1.0 << 0.0 << 6.0;
    QTest::newRow("neg1") << -1.0 << -6.0 << 0.0;
    QTest::newRow("pos2") << 2.0 << 0.0 << 12.0;
    QTest::newRow("neg10") << -10.0 << -60.0 << 0.0;
}

void TestDataLayer::testCalculateHeightLimitsNull()
{
    QFETCH(double, heightDimension);
    QFETCH(double, lowerLimit);
    QFETCH(double, upperLimit);
    
    m_geometry.setHeightDimension(heightDimension);
    m_dataLayer.setGeometry(m_geometry);
    
    int realLowHeightTrim = -100;
    int realHighHeightTrim = -100;
    QList<CoordinateAxis> axes;
    
    m_dataLayer.calculateHeightLimits(0, &realLowHeightTrim, &realHighHeightTrim, &axes, DataLayer::Contains);
    QCOMPARE(realLowHeightTrim, 0);
    QCOMPARE(realHighHeightTrim, 6);
    QCOMPARE(axes.size(), 1);
    QCOMPARE(axes.at(0).lowerLimit().toDouble(), lowerLimit);
    QCOMPARE(axes.at(0).upperLimit().toDouble(), upperLimit);
    realLowHeightTrim = -100;
    realHighHeightTrim = -100;
    
    m_dataLayer.calculateHeightLimits(0, &realLowHeightTrim, &realHighHeightTrim, &axes, DataLayer::Overlaps);
    QCOMPARE(realLowHeightTrim, 0);
    QCOMPARE(realHighHeightTrim, 6);
    QCOMPARE(axes.size(), 2);
    QCOMPARE(axes.at(1).lowerLimit().toDouble(), lowerLimit);
    QCOMPARE(axes.at(1).upperLimit().toDouble(), upperLimit);
}

QTEST_MAIN( TestDataLayer )
#include "TestDataLayer.moc"
