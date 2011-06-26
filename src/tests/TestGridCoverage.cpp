//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "CoordinateAxis.h"
#include "GridCoverage.h"

class TestGridCoverage : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void test();
};

void TestGridCoverage::test()
{
    GridCoverage original;
    QList<CoordinateAxis> axes;
    axes.append(CoordinateAxis(Lat));
    axes.append(CoordinateAxis(Lon));
    original.setCoordinateAxes(axes);
    double *values1 = new double[5];
    original.setValues(values1);

    QCOMPARE(original.coordinateAxes(), axes);
    QCOMPARE(original.values(), values1);
}

QTEST_MAIN( TestGridCoverage )
#include "TestGridCoverage.moc"
