//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QObject>
#include <QtTest/QtTest>

// Project
#include "DimensionSlice.h"
#include "BadDimensionTypeException.h"

class TestDimensionSlice : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void testCopy();
    void testAssignment();

    void testBadType();
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

void TestDimensionSlice::testBadType()
{
    DimensionSlice timeSlice(Time);

    QVariant oldValue = timeSlice.slicePoint();
    bool exceptionThrown = false;
    try {
        timeSlice.setSlicePoint(10.0);
    }
    catch (BadDimensionTypeException exception) {
        exceptionThrown = true;
    }
    QCOMPARE(exceptionThrown, true);
    QCOMPARE(timeSlice.slicePoint(), oldValue);

    DimensionSlice lonSlice(Lon);

    oldValue = lonSlice.slicePoint();
    exceptionThrown = false;
    try {
        lonSlice.setSlicePoint(QDateTime());
    }
    catch (BadDimensionTypeException exception) {
        exceptionThrown = true;
    }
    QCOMPARE(exceptionThrown, true);
    QCOMPARE(lonSlice.slicePoint(), oldValue);
}

QTEST_MAIN( TestDimensionSlice )
#include "TestDimensionSlice.moc"
