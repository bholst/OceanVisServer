//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "CoordinateAxis.h"

class TestCoordinateAxis : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void testCopy();
    void testAssignment();
    void testBadType();
};

void TestCoordinateAxis::testCopy()
{
    CoordinateAxis original(Lat);
    original.setMinValue(-50.0);
    original.setMaxValue(-20.0);
    original.setValueCount(100);

    CoordinateAxis other(original);

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(original.minValue().toDouble(), -50.0);
    QCOMPARE(original.maxValue().toDouble(), -20.0);
    QCOMPARE(original.valueCount(), 100);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(other.minValue().toDouble(), -50.0);
    QCOMPARE(other.maxValue().toDouble(), -20.0);
    QCOMPARE(other.valueCount(), 100);

    other.setMinValue(-60.0);
    original.setMaxValue(-10.0);
    other.setValueCount(20);

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(original.minValue().toDouble(), -50.0);
    QCOMPARE(original.maxValue().toDouble(), -10.0);
    QCOMPARE(original.valueCount(), 100);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(other.minValue().toDouble(), -60.0);
    QCOMPARE(other.maxValue().toDouble(), -20.0);
    QCOMPARE(other.valueCount(), 20);
}

void TestCoordinateAxis::testAssignment()
{
    CoordinateAxis original(Lat);
    original.setMinValue(-50.0);
    original.setMaxValue(-20.0);
    original.setValueCount(100);

    CoordinateAxis other(Lon);
    other = original;
    
    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(original.minValue().toDouble(), -50.0);
    QCOMPARE(original.maxValue().toDouble(), -20.0);
    QCOMPARE(original.valueCount(), 100);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(other.minValue().toDouble(), -50.0);
    QCOMPARE(other.maxValue().toDouble(), -20.0);
    QCOMPARE(other.valueCount(), 100);

    original.setMaxValue(-10.0);
    other.setMinValue(-60.0);
    other.setValueCount(20);

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(original.minValue().toDouble(), -50.0);
    QCOMPARE(original.maxValue().toDouble(), -10.0);
    QCOMPARE(original.valueCount(), 100);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(other.minValue().toDouble(), -60.0);
    QCOMPARE(other.maxValue().toDouble(), -20.0);
    QCOMPARE(other.valueCount(), 20);
}

void TestCoordinateAxis::testBadType()
{
    CoordinateAxis timeAxis(Time);

    QVariant oldValue = timeAxis.minValue();
    bool exceptionThrown = false;
    try {
        timeAxis.setMinValue(10.0);
    }
    catch (BadDimensionTypeException exception) {
        exceptionThrown = true;
    }
    QCOMPARE(exceptionThrown, true);
    QCOMPARE(timeAxis.minValue(), oldValue);

    CoordinateAxis lonAxis(Lon);

    oldValue = lonAxis.maxValue();
    exceptionThrown = false;
    try {
        lonAxis.setMaxValue(QDateTime());
    }
    catch (BadDimensionTypeException exception) {
        exceptionThrown = true;
    }
    QCOMPARE(exceptionThrown, true);
    QCOMPARE(lonAxis.maxValue(), oldValue);
}

QTEST_MAIN( TestCoordinateAxis )
#include "TestCoordinateAxis.moc"
