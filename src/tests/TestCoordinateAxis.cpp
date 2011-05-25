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
    original.setMinimumValue(-50.0);
    original.setMaximumValue(-20.0);
    original.setValueCount(100);

    CoordinateAxis other(original);

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(original.minimumValue().toDouble(), -50.0);
    QCOMPARE(original.maximumValue().toDouble(), -20.0);
    QCOMPARE(original.valueCount(), 100);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(other.minimumValue().toDouble(), -50.0);
    QCOMPARE(other.maximumValue().toDouble(), -20.0);
    QCOMPARE(other.valueCount(), 100);

    other.setMinimumValue(-60.0);
    original.setMaximumValue(-10.0);
    other.setValueCount(20);

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(original.minimumValue().toDouble(), -50.0);
    QCOMPARE(original.maximumValue().toDouble(), -10.0);
    QCOMPARE(original.valueCount(), 100);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(other.minimumValue().toDouble(), -60.0);
    QCOMPARE(other.maximumValue().toDouble(), -20.0);
    QCOMPARE(other.valueCount(), 20);
}

void TestCoordinateAxis::testAssignment()
{
    CoordinateAxis original(Lat);
    original.setMinimumValue(-50.0);
    original.setMaximumValue(-20.0);
    original.setValueCount(100);

    CoordinateAxis other(Lon);
    other = original;
    
    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(original.minimumValue().toDouble(), -50.0);
    QCOMPARE(original.maximumValue().toDouble(), -20.0);
    QCOMPARE(original.valueCount(), 100);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(other.minimumValue().toDouble(), -50.0);
    QCOMPARE(other.maximumValue().toDouble(), -20.0);
    QCOMPARE(other.valueCount(), 100);

    original.setMaximumValue(-10.0);
    other.setMinimumValue(-60.0);
    other.setValueCount(20);

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(original.minimumValue().toDouble(), -50.0);
    QCOMPARE(original.maximumValue().toDouble(), -10.0);
    QCOMPARE(original.valueCount(), 100);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(other.minimumValue().toDouble(), -60.0);
    QCOMPARE(other.maximumValue().toDouble(), -20.0);
    QCOMPARE(other.valueCount(), 20);
}

void TestCoordinateAxis::testBadType()
{
    CoordinateAxis timeAxis(Time);

    QVariant oldValue = timeAxis.minimumValue();
    bool exceptionThrown = false;
    try {
        timeAxis.setMinimumValue(10.0);
    }
    catch (BadDimensionTypeException exception) {
        exceptionThrown = true;
    }
    QCOMPARE(exceptionThrown, true);
    QCOMPARE(timeAxis.minimumValue(), oldValue);

    CoordinateAxis lonAxis(Lon);

    oldValue = lonAxis.maximumValue();
    exceptionThrown = false;
    try {
        lonAxis.setMaximumValue(QDateTime());
    }
    catch (BadDimensionTypeException exception) {
        exceptionThrown = true;
    }
    QCOMPARE(exceptionThrown, true);
    QCOMPARE(lonAxis.maximumValue(), oldValue);
}

QTEST_MAIN( TestCoordinateAxis )
#include "TestCoordinateAxis.moc"
