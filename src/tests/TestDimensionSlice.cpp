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
    DimensionSlice original;
    original.setDimension(DimensionSubset::Lat);
    original.setSlicePoint(-70.0);
    DimensionSlice other(original);

    QCOMPARE(original.dimension(), DimensionSubset::Lat);
    QCOMPARE(other.dimension(), DimensionSubset::Lat);
    QCOMPARE(original.slicePoint().toDouble(), -70.0);
    QCOMPARE(other.slicePoint().toDouble(), -70.0);

    other.setDimension(DimensionSubset::Time);
    other.setSlicePoint(20.0);

    QCOMPARE(original.dimension(), DimensionSubset::Lat);
    QCOMPARE(other.dimension(), DimensionSubset::Time);
    QCOMPARE(original.slicePoint().toDouble(), -70.0);
    QCOMPARE(other.slicePoint().toDouble(), 20.0);
}

void TestDimensionSlice::testAssignment()
{
    DimensionSlice original;
    original.setDimension(DimensionSubset::Lat);
    original.setSlicePoint(-70.0);

    DimensionSlice other;
    other = original;

    QCOMPARE(original.dimension(), DimensionSubset::Lat);
    QCOMPARE(other.dimension(), DimensionSubset::Lat);
    QCOMPARE(original.slicePoint().toDouble(), -70.0);
    QCOMPARE(other.slicePoint().toDouble(), -70.0);

    other.setDimension(DimensionSubset::Time);
    other.setSlicePoint(20.0);

    QCOMPARE(original.dimension(), DimensionSubset::Lat);
    QCOMPARE(other.dimension(), DimensionSubset::Time);
    QCOMPARE(original.slicePoint().toDouble(), -70.0);
    QCOMPARE(other.slicePoint().toDouble(), 20.0);
}

QTEST_MAIN( TestDimensionSlice )
#include "TestDimensionSlice.moc"
