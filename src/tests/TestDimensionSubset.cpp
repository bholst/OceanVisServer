//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "DimensionSubset.h"

class TestDimensionSubset : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void testCopy();
    void testAssignment();
};

void TestDimensionSubset::testCopy()
{
    DimensionSubset original;
    original.setDimension(DimensionSubset::Lat);
    DimensionSubset other(original);

    QCOMPARE(original.dimension(), DimensionSubset::Lat);
    QCOMPARE(other.dimension(), DimensionSubset::Lat);

    other.setDimension(DimensionSubset::Time);

    QCOMPARE(original.dimension(), DimensionSubset::Lat);
    QCOMPARE(other.dimension(), DimensionSubset::Time);
}

void TestDimensionSubset::testAssignment()
{
    DimensionSubset original;
    original.setDimension(DimensionSubset::Lat);

    DimensionSubset other;
    other = original;

    QCOMPARE(original.dimension(), DimensionSubset::Lat);
    QCOMPARE(other.dimension(), DimensionSubset::Lat);

    other.setDimension(DimensionSubset::Time);

    QCOMPARE(original.dimension(), DimensionSubset::Lat);
    QCOMPARE(other.dimension(), DimensionSubset::Time);
}

QTEST_MAIN( TestDimensionSubset )
#include "TestDimensionSubset.moc"
