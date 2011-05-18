//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "DimensionTrim.h"

class TestDimensionTrim : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void testCopy();
    void testAssignment();
};

void TestDimensionTrim::testCopy()
{
    DimensionTrim original;
    original.setDimension(DimensionSubset::Lat);
    original.setTrimLow(-70.0);
    original.setTrimHigh(88.0);
    DimensionTrim other(original);

    QCOMPARE(original.dimension(), DimensionSubset::Lat);
    QCOMPARE(other.dimension(), DimensionSubset::Lat);
    QCOMPARE(original.trimLow().toDouble(), -70.0);
    QCOMPARE(other.trimLow().toDouble(), -70.0);
    QCOMPARE(original.trimHigh().toDouble(), 88.0);
    QCOMPARE(other.trimHigh().toDouble(), 88.0);

    other.setDimension(DimensionSubset::Time);
    other.setTrimLow(20.0);
    other.setTrimHigh(30.0);

    QCOMPARE(original.dimension(), DimensionSubset::Lat);
    QCOMPARE(other.dimension(), DimensionSubset::Time);
    QCOMPARE(original.trimLow().toDouble(), -70.0);
    QCOMPARE(other.trimLow().toDouble(), 20.0);
    QCOMPARE(original.trimHigh().toDouble(), 88.0);
    QCOMPARE(other.trimHigh().toDouble(), 30.0);
}

void TestDimensionTrim::testAssignment()
{
    DimensionTrim original;
    original.setDimension(DimensionSubset::Lat);
    original.setTrimLow(-70.0);
    original.setTrimHigh(88.0);

    DimensionTrim other;
    other = original;

    QCOMPARE(original.dimension(), DimensionSubset::Lat);
    QCOMPARE(other.dimension(), DimensionSubset::Lat);
    QCOMPARE(original.trimLow().toDouble(), -70.0);
    QCOMPARE(other.trimLow().toDouble(), -70.0);
    QCOMPARE(original.trimHigh().toDouble(), 88.0);
    QCOMPARE(other.trimHigh().toDouble(), 88.0);

    other.setDimension(DimensionSubset::Time);
    other.setTrimLow(20.0);
    other.setTrimHigh(30.0);

    QCOMPARE(original.dimension(), DimensionSubset::Lat);
    QCOMPARE(other.dimension(), DimensionSubset::Time);
    QCOMPARE(original.trimLow().toDouble(), -70.0);
    QCOMPARE(other.trimLow().toDouble(), 20.0);
    QCOMPARE(original.trimHigh().toDouble(), 88.0);
    QCOMPARE(other.trimHigh().toDouble(), 30.0);
}

QTEST_MAIN( TestDimensionTrim )
#include "TestDimensionTrim.moc"
