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
    DimensionSubset original(Lat);
    DimensionSubset other(original);

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(other.dimension(), Lat);
}

void TestDimensionSubset::testAssignment()
{
    DimensionSubset original(Lat);

    DimensionSubset other(Lon);
    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(other.dimension(), Lon);

    other = original;

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(other.dimension(), Lat);
}

QTEST_MAIN( TestDimensionSubset )
#include "TestDimensionSubset.moc"
