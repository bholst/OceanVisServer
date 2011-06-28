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
     * Set the minimum value on this axis.
     * In the described matrix is a value for minimumValue,
     * but there is no value for smaller points in this dimension.
     *
     * @param minimumValue The minimum value on this axis. The QVariant has to
     *                     contain a type which is convertible to the type
     *                     associated to the dimension of this object.
     */
    void setMinValue(const QVariant& minValue) throw (BadDimensionTypeException);
    /**
     * Return the maximum value on this axis.
     * In the described matrix is a value for minimumValue,
     * but there is no value for smaller points in this dimension.
     */
    QVariant minValue() const;

    /**
     * Set the maximum value on this axis.
     * In the described matrix is a value for maximumValue,
     * but there is no value for higher points in this dimension.
     *
     * @param maximumValue The maximum value on this axis. The QVariant has to
     *                     contain a type which is convertible to the type
     *                     associated to the dimension of this object.
     */
    void setMaxValue(const QVariant& maxValue) throw (BadDimensionTypeException);
    /**
     * Return the maximum value on this axis.
     * In the described matrix is a value for maximumValue,
     * but there is no value for higher points in this dimension.
     */
    QVariant maxValue() const;

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
