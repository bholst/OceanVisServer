//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef COORDINATEAXIS_H
#define COORDINATEAXIS_H

// Project
#include "BadDimensionTypeException.h"

class QVariant;

class CoordinateAxisPrivate;

/**
 * The objects of the class @class CoordinateAxis represent an axis in
 * @class DataMatrix. It will typically be given along with the data
 * subsets to provide semantics for the user of the data.
 * This class is implicitly shared so it is very cheap to copy it.
 */
class CoordinateAxis {
public:
    /**
     * Create a coordinate axis for the Dimension @p dimension.
     *
     * @param dimension The dimension to be used.
     */
    CoordinateAxis(Dimension dimension);
    CoordinateAxis(const CoordinateAxis &other);
    ~CoordinateAxis();

    /**
     * Return the dimension of the axis.
     * The dimension cannot be changed once an object is created to ensure
     * valid types in all fields.
     */
    Dimension dimension() const;

    /**
     * Set the lower limit on this axis.
     * The described matrix contains values for values with a larger coordinate
     * on this axis but not for lower ones.
     *
     * @param lowerLimit The lower limit of this axis. The QVariant has to
     *                   contain a type which is convertible to the type
     *                   associated to the dimension of this object.
     */
    void setLowerLimit(const QVariant& lowerLimit) throw (BadDimensionTypeException);
    /**
     * Return the lower limit on this axis.
     * The described matrix contains values for values with a larger coordinate
     * on this axis but not for lower ones.
     */
    QVariant lowerLimit() const;

    /**
     * Set the upper limit on this axis.
     * The described matrix contains values for values with a lower coordinate
     * on this axis but not for higher ones.
     *
     * @param upperLimit The maximum value on this axis. The QVariant has to
     *                     contain a type which is convertible to the type
     *                     associated to the dimension of this object.
     */
    void setUpperLimit(const QVariant& upperLimit) throw (BadDimensionTypeException);
    /**
     * Return the upper limit on this axis.
     * The described matrix contains values for values with a lower coordinate
     * on this axis but not for higher ones.
     */
    QVariant upperLimit() const;

    /**
     * Set the number of points on this axis.
     */
    void setValueCount(int valueCount);
    /**
     * Get the number of points on this axis.
     */
    int valueCount() const;

    CoordinateAxis& operator=(const CoordinateAxis &other);

    bool operator==(const CoordinateAxis &other);

    void detach();

private:
    CoordinateAxisPrivate * d;
};

#endif
