//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "MapGeometry.h"
#include "MapGeometryParser.h"

class TestMapGeometryParser : public QObject {
    Q_OBJECT
public:
    TestMapGeometryParser();
    ~TestMapGeometryParser();

private Q_SLOTS:
    void initTestCase();

    void testParsing_data();
    void testParsing();

private:
    MapGeometry *m_geometry;
};

TestMapGeometryParser::TestMapGeometryParser()
    : m_geometry(0)
{
}

TestMapGeometryParser::~TestMapGeometryParser()
{
    delete m_geometry;
}

void TestMapGeometryParser::initTestCase()
{
    QString file;
    file.append("# Created by Octave 3.2.4, Wed May 11 12:56:13 2011 CEST <bastian@snotra>\n");
    file.append("# name: ideep\n");
    file.append("# type: matrix\n");
    file.append("# rows: 2\n");
    file.append("# columns: 4\n");
    file.append(" 1 2 0 3\n");
    file.append(" 4 0 5 6\n");
    
    MapGeometryParser parser;
    QTextStream stream(&file, QIODevice::ReadOnly);
    parser.setTextStream(&stream);
    m_geometry = parser.mapGeometry();
}

void TestMapGeometryParser::testParsing_data()
{
    QTest::addColumn<int>("x");
    QTest::addColumn<int>("y");
    QTest::addColumn<int>("layerCount");
    QTest::addColumn<long>("start");

    QTest::newRow("col0row0") << 0 << 0 << 1 << (long) 0;
    QTest::newRow("col1row0") << 1 << 0 << 2 << (long) 1;
    QTest::newRow("col3row1") << 3 << 1 << 6 << (long) 15;
}

void TestMapGeometryParser::testParsing()
{
    QFETCH(int, x);
    QFETCH(int, y);
    QFETCH(int, layerCount);
    QFETCH(long, start);

    QCOMPARE(m_geometry->layerCount(x, y), layerCount);
    QCOMPARE(m_geometry->start(x, y), start);
}

QTEST_MAIN( TestMapGeometryParser )
#include "TestMapGeometryParser.moc"
