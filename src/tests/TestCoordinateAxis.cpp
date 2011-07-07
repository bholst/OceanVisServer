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
    original.setLowerLimit(-50.0);
    original.setUpperLimit(-20.0);
    original.setValueCount(100);

    CoordinateAxis other(original);

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(original.lowerLimit().toDouble(), -50.0);
    QCOMPARE(original.upperLimit().toDouble(), -20.0);
    QCOMPARE(original.valueCount(), 100);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(other.lowerLimit().toDouble(), -50.0);
    QCOMPARE(other.upperLimit().toDouble(), -20.0);
    QCOMPARE(other.valueCount(), 100);

    other.setLowerLimit(-60.0);
    original.setUpperLimit(-10.0);
    other.setValueCount(20);

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(original.lowerLimit().toDouble(), -50.0);
    QCOMPARE(original.upperLimit().toDouble(), -10.0);
    QCOMPARE(original.valueCount(), 100);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(other.lowerLimit().toDouble(), -60.0);
    QCOMPARE(other.upperLimit().toDouble(), -20.0);
    QCOMPARE(other.valueCount(), 20);
}

void TestCoordinateAxis::testAssignment()
{
    CoordinateAxis original(Lat);
    original.setLowerLimit(-50.0);
    original.setUpperLimit(-20.0);
    original.setValueCount(100);

    CoordinateAxis other(Lon);
    other = original;
    
    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(original.lowerLimit().toDouble(), -50.0);
    QCOMPARE(original.upperLimit().toDouble(), -20.0);
    QCOMPARE(original.valueCount(), 100);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(other.lowerLimit().toDouble(), -50.0);
    QCOMPARE(other.upperLimit().toDouble(), -20.0);
    QCOMPARE(other.valueCount(), 100);

    original.setUpperLimit(-10.0);
    other.setLowerLimit(-60.0);
    other.setValueCount(20);

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(original.lowerLimit().toDouble(), -50.0);
    QCOMPARE(original.upperLimit().toDouble(), -10.0);
    QCOMPARE(original.valueCount(), 100);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(other.lowerLimit().toDouble(), -60.0);
    QCOMPARE(other.upperLimit().toDouble(), -20.0);
    QCOMPARE(other.valueCount(), 20);
}

void TestCoordinateAxis::testBadType()
{
    CoordinateAxis timeAxis(Time);

    QVariant oldValue = timeAxis.lowerLimit();
    bool exceptionThrown = false;
    try {
        timeAxis.setLowerLimit(10.0);
    }
    catch (BadDimensionTypeException exception) {
        exceptionThrown = true;
    }
    QCOMPARE(exceptionThrown, true);
    QCOMPARE(timeAxis.lowerLimit(), oldValue);

    CoordinateAxis lonAxis(Lon);

    oldValue = lonAxis.upperLimit();
    exceptionThrown = false;
    try {
        lonAxis.setUpperLimit(QDateTime());
    }
    catch (BadDimensionTypeException exception) {
        exceptionThrown = true;
    }
    QCOMPARE(exceptionThrown, true);
    QCOMPARE(lonAxis.upperLimit(), oldValue);
}

QTEST_MAIN( TestCoordinateAxis )
#include "TestCoordinateAxis.moc"
