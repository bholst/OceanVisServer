//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DATALAYER_H
#define DATALAYER_H

// Qt
#include <QtCore/QList>

class GridCoverage;
class DimensionSubset;
class MapGeometry;
class CoordinateAxis;
class Constant;
class ColorMap;
class QString;
class QFile;
class QDateTime;

class DataLayerPrivate;

class DataLayer {
public:
    enum CutMode {
        Contains,
        Overlaps
    };
    
    DataLayer();
    virtual ~DataLayer();

    void setName(const QString& name);
    QString name() const;

    void setFileName(const QDateTime& dateTime, const QString& fileName);
    QString fileName(const QDateTime& dateTime) const;

    MapGeometry geometry() const;
    void setGeometry(const MapGeometry &mapGeometry);
    
    /**
     * Set a fixed maximum of the scale.
     */
    void setScaleMax(double scaleMax);
    double scaleMax() const;
    /**
     * Returns the maximum value to be used for the scale.
     * This is either the value set as scaleMaximum
     * or, if scaleMaximum and scaleMinimum are equal, the maximum of
     * all values in all files.
     */
    double maxValue() const;
    
    /**
     * Set a fixed minimum of the scale.
     */
    void setScaleMin(double scaleMin);
    double scaleMin() const;
    /**
     * Returns the minumum value to be used for the scale.
     * This is either the value set as scaleMinimum
     * of, if scaleMaximum and scaleMinimum are equal, the minimum of
     * all values in all files.
     */
    double minValue() const;
    
    /**
     * Set the default color map for this layer.
     * This is the color map to be used if the user does not specify another color map.
     */
    void setDefaultColorMap(const ColorMap& defaultColorMap);
    
    /**
     * Return the color map to be used for images if the user does not specify a color map.
     */
    ColorMap defaultColorMap() const;
    
    /**
     * Returns the coordinateAxes of this DataLayer which are these axes on which you
     * can trim and slice.
     */
    QList<CoordinateAxis> coordinateAxes() const;
    
    /**
     * Returns the constants of this DataLayer which are these axes which are already 
     * sliced in the data.
     */
    QList<Constant> constants() const;

    GridCoverage *dataSubset(QList<DimensionSubset*>& subsets, CutMode mode = Contains);
    
    void calculateTimeLimits(DimensionSubset *subset,
                             QMap<QDateTime,double*>::const_iterator *lowTimeTrim, 
                             QMap<QDateTime,double*>::const_iterator *highTimeTrim,
                             int *dimensionCount,
                             QList<CoordinateAxis> *axes, QList<Constant> *consts,
                             CutMode mode);
    void calculateLonLimits(DimensionSubset *subset,
                            int *lowLonTrim, int *highLonTrim,
                            QList<CoordinateAxis> *axes, QList<Constant> *consts,
                            CutMode mode);
    void calculateLatLimits(DimensionSubset *subset,
                            int *lowLatTrim, int *highLatTrim,
                            QList<CoordinateAxis> *axes, QList<Constant> *consts,
                            CutMode mode);
    void calculateHeightLimits(DimensionSubset *subset,
                               int *lowHeightTrim, int *highHeightTrim,
                               QList<CoordinateAxis> *axes, QList<Constant> *consts,
                               CutMode mode);

private:
    Q_DISABLE_COPY(DataLayer);

    DataLayerPrivate * const d;
};

#endif
