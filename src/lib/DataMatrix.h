//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DATAMATRIX_H
#define DATAMATRIX_H

#include <QtCore/QList>

class CoordinateAxis;
class QString;

class DataMatrixPrivate;

class DataMatrix {
public:
    DataMatrix();
    ~DataMatrix();

    void setCoordinateAxes(const QList<CoordinateAxis>& axes);
    QList<CoordinateAxis> coordinateAxes() const;

    /**
     * Set the value vector.
     * Data matrix will take the ownership of values.
     */
    void setValues(double *values);
    double *values();

    void setMaxValue(double maxValue);
    double maxValue();

    QString toString() const;

    QImage toImage() const;

private:
    Q_DISABLE_COPY(DataMatrix)
    DataMatrixPrivate * const d;
};

#endif
