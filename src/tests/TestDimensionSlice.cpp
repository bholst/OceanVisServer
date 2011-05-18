//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "DimensionSlice.h"

class TestDimensionSlice : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void testCopy();
    void testAssignment();
};

void TestDimensionSlice::testCopy()
{
    DimensionSlice original(Lat);
    original.setSlicePoint(-70.0);
    DimensionSlice other(original);

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(original.slicePoint().toDouble(), -70.0);
    QCOMPARE(other.slicePoint().toDouble(), -70.0);

    other.setSlicePoint(20.0);

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(original.slicePoint().toDouble(), -70.0);
    QCOMPARE(other.slicePoint().toDouble(), 20.0);
}

void TestDimensionSlice::testAssignment()
{
    DimensionSlice original(Lat);
    original.setSlicePoint(-70.0);

    DimensionSlice other(Lon);
    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(other.dimension(), Lon);

    other = original;

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(original.slicePoint().toDouble(), -70.0);
    QCOMPARE(other.slicePoint().toDouble(), -70.0);

    other.setSlicePoint(20.0);

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(original.slicePoint().toDouble(), -70.0);
    QCOMPARE(other.slicePoint().toDouble(), 20.0);
}

QTEST_MAIN( TestDimensionSlice )
#include "TestDimensionSlice.moc"
