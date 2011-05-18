//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QObject>
#include <QtTest/QtTest>

// Projec
#include "DimensionTrim.h"
#include "BadDimensionTypeException.h"

class TestDimensionTrim : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void testCopy();
    void testAssignment();
    void testBadType();
};

void TestDimensionTrim::testCopy()
{
    DimensionTrim original(Lat);
    original.setTrimLow(-70.0);
    original.setTrimHigh(88.0);
    DimensionTrim other(original);

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(original.trimLow().toDouble(), -70.0);
    QCOMPARE(other.trimLow().toDouble(), -70.0);
    QCOMPARE(original.trimHigh().toDouble(), 88.0);
    QCOMPARE(other.trimHigh().toDouble(), 88.0);

    other.setTrimLow(20.0);
    other.setTrimHigh(30.0);

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(original.trimLow().toDouble(), -70.0);
    QCOMPARE(other.trimLow().toDouble(), 20.0);
    QCOMPARE(original.trimHigh().toDouble(), 88.0);
    QCOMPARE(other.trimHigh().toDouble(), 30.0);
}

void TestDimensionTrim::testAssignment()
{
    DimensionTrim original(Lat);
    original.setTrimLow(-70.0);
    original.setTrimHigh(88.0);

    DimensionTrim other(Lon);
    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(other.dimension(), Lon);

    other = original;

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(original.trimLow().toDouble(), -70.0);
    QCOMPARE(other.trimLow().toDouble(), -70.0);
    QCOMPARE(original.trimHigh().toDouble(), 88.0);
    QCOMPARE(other.trimHigh().toDouble(), 88.0);

    other.setTrimLow(20.0);
    other.setTrimHigh(30.0);

    QCOMPARE(original.dimension(), Lat);
    QCOMPARE(other.dimension(), Lat);
    QCOMPARE(original.trimLow().toDouble(), -70.0);
    QCOMPARE(other.trimLow().toDouble(), 20.0);
    QCOMPARE(original.trimHigh().toDouble(), 88.0);
    QCOMPARE(other.trimHigh().toDouble(), 30.0);
}

void TestDimensionTrim::testBadType()
{
    DimensionTrim timeSlice(Time);

    QVariant oldValue = timeSlice.trimLow();
    bool exceptionThrown = false;
    try {
        timeSlice.setTrimLow(10.0);
    }
    catch (BadDimensionTypeException exception) {
        exceptionThrown = true;
    }
    QCOMPARE(exceptionThrown, true);
    QCOMPARE(timeSlice.trimLow(), oldValue);

    DimensionTrim lonSlice(Lon);

    oldValue = lonSlice.trimHigh();
    exceptionThrown = false;
    try {
        lonSlice.setTrimHigh(QDateTime());
    }
    catch (BadDimensionTypeException exception) {
        exceptionThrown = true;
    }
    QCOMPARE(exceptionThrown, true);
    QCOMPARE(lonSlice.trimHigh(), oldValue);
}

QTEST_MAIN( TestDimensionTrim )
#include "TestDimensionTrim.moc"
