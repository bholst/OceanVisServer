//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "MapGeometry.h"

class TestMapGeometry : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();

    void testMapGeometry_data();
    void testMapGeometry();

    void testTotalSize();
    
    void testCopy();
    void testAssignment();

private:
    MapGeometry m_geometry;
};

void TestMapGeometry::initTestCase()
{
    m_geometry.setWidth(4);
    m_geometry.setHeight(2);

    int layerCounts[] = {1, 2, 0, 3,
                         4, 0, 5, 6};
    m_geometry.setLayerCounts(layerCounts);
}

void TestMapGeometry::testMapGeometry_data()
{
    QTest::addColumn<int>("x");
    QTest::addColumn<int>("y");
    QTest::addColumn<int>("layerCount");
    QTest::addColumn<long>("start");

    QTest::newRow("col0row0") << 0 << 0 << 1 << (long) 0;
    QTest::newRow("col1row0") << 1 << 0 << 2 << (long) 1;
    QTest::newRow("col3row1") << 3 << 1 << 6 << (long) 15;
}

void TestMapGeometry::testMapGeometry()
{
    QFETCH(int, x);
    QFETCH(int, y);
    QFETCH(int, layerCount);
    QFETCH(long, start);

    QCOMPARE(m_geometry.layerCount(x, y), layerCount);
    QCOMPARE(m_geometry.start(x, y), start);
}

void TestMapGeometry::testTotalSize()
{
    QCOMPARE(m_geometry.totalSize(), (long) 21);
}

void TestMapGeometry::testCopy()
{
    MapGeometry other(m_geometry);
    
    QCOMPARE(m_geometry.width(), 4);
    QCOMPARE(other.width(), 4);
    QCOMPARE(m_geometry.height(), 2);
    QCOMPARE(other.height(), 2);
    
    other.setHeight(5);
    other.setWidth(3);
    
    QCOMPARE(m_geometry.width(), 4);
    QCOMPARE(other.width(), 3);
    QCOMPARE(m_geometry.height(), 2);
    QCOMPARE(other.height(), 5);
}

void TestMapGeometry::testAssignment()
{
    MapGeometry other;
    other.setWidth(1);
    other.setHeight(1);
    
    QCOMPARE(m_geometry.width(), 4);
    QCOMPARE(other.width(), 1);
    QCOMPARE(m_geometry.height(), 2);
    QCOMPARE(other.height(), 1);
    
    other = m_geometry;
    
    QCOMPARE(m_geometry.width(), 4);
    QCOMPARE(other.width(), 4);
    QCOMPARE(m_geometry.height(), 2);
    QCOMPARE(other.height(), 2);
    
    other.setHeight(5);
    other.setWidth(3);
    
    QCOMPARE(m_geometry.width(), 4);
    QCOMPARE(other.width(), 3);
    QCOMPARE(m_geometry.height(), 2);
    QCOMPARE(other.height(), 5);
}

QTEST_MAIN( TestMapGeometry )
#include "TestMapGeometry.moc"
