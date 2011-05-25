//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "CoordinateAxis.h"
#include "DataMatrix.h"

class TestDataMatrix : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void test();
};

void TestDataMatrix::test()
{
    DataMatrix original;
    QList<CoordinateAxis> axes;
    axes.append(CoordinateAxis(Lat));
    axes.append(CoordinateAxis(Lon));
    original.setCoordinateAxes(axes);
    double *values1;
    original.setValues(values1);

    QCOMPARE(original.coordinateAxes(), axes);
    QCOMPARE(original.values(), values1);
}

QTEST_MAIN( TestDataMatrix )
#include "TestDataMatrix.moc"
