//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef COVERAGE_H
#define COVERAGE_H

#include <QtCore/QString>
#include <QtCore/QList>

class Constant;
class CoordinateAxis;

class Coverage {
public:
    Coverage();
    ~Coverage();
    
    void setCoverageId(const QString& coverageId);
    QString coverageId() const;
    
    void setConstants(const QList<Constant>& constants);
    QList<Constant> constants() const;
    
    void setCoordinateAxes(const QList<CoordinateAxis>& axes);
    QList<CoordinateAxis> coordinateAxes() const;
    
private:
    QString m_coverageId;
    QList<Constant> m_constants;
    QList<CoordinateAxis> m_axes;
};

#endif
