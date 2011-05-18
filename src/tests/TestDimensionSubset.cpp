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
    DimensionSubset original(DimensionSubset::Lat);
    DimensionSubset other(original);

    QCOMPARE(original.dimension(), DimensionSubset::Lat);
    QCOMPARE(other.dimension(), DimensionSubset::Lat);
}

void TestDimensionSubset::testAssignment()
{
    DimensionSubset original(DimensionSubset::Lat);

    DimensionSubset other(DimensionSubset::Lon);
    QCOMPARE(original.dimension(), DimensionSubset::Lat);
    QCOMPARE(other.dimension(), DimensionSubset::Lon);

    other = original;

    QCOMPARE(original.dimension(), DimensionSubset::Lat);
    QCOMPARE(other.dimension(), DimensionSubset::Lat);
}

QTEST_MAIN( TestDimensionSubset )
#include "TestDimensionSubset.moc"
