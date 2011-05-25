//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DATAMATRIX_H
#define DATAMATRIX_H

#include <QtCore/QList>

class CoordinateAxis;

class DataMatrixPrivate;

class DataMatrix {
public:
    DataMatrix();
    ~DataMatrix();

    void setCoordinateAxes(const QList<CoordinateAxis>& axes);
    QList<CoordinateAxis> coordinateAxes() const;

    void setValues(double *values);
    double *values();

private:
    Q_DISABLE_COPY(DataMatrix)
    DataMatrixPrivate * const d;
};

#endif
