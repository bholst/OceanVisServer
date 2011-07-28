//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef CONSTANT_H
#define CONSTANT_H

// Project
#include "BadDimensionTypeException.h"

class QVariant;

class ConstantPrivate;

/**
 * The objects of the class @class Constant represent an contant in
 * @class DataMatrix. It will typically be given along with the data
 * subsets to provide semantics for the user of the data.
 * This is the direct counterpart of an axis, it just means that there
 * is no axis, so we already have a slice on this axis.
 * This class is implicitly shared so it is very cheap to copy it.
 */
class Constant {
public:
    /**
     * Create a constant for the Dimension @p dimension.
     *
     * @param dimension The dimension to be used.
     */
    Constant(Dimension dimension);
    Constant(const Constant &other);
    ~Constant();

    /**
     * Return the dimension of the constant.
     * The dimension cannot be changed once an object is created to ensure
     * valid types in all fields.
     */
    Dimension dimension() const;

    /**
     * Set the value.
     * This value is an additional information for each value in the described matrix.
     *
     */
    void setValue(const QVariant& lowerLimit) throw (BadDimensionTypeException);
    /**
     * Return the value.
     * This value is an additional information for each value in the described matrix.
     */
    QVariant value() const;

    Constant& operator=(const Constant &other);

    bool operator==(const Constant &other);

    void detach();

private:
    ConstantPrivate * d;
};

#endif
