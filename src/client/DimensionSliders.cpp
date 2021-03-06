//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <cmath>

// Qt
#include <QtCore/QDebug>
#include <QtCore/QList>

// Project
#include "Coverage.h"
#include "CoordinateAxis.h"
#include "Constant.h"

// Self
#include "DimensionSliders.h"

DimensionSliders::DimensionSliders(QWidget *parent) 
    : QWidget(parent),
      m_mainWindow(0),
      m_valueCountTime(0),
      m_valueCountLon(0),
      m_valueCountLat(0),
      m_minLon(0.0),
      m_maxLon(0.0),
      m_minLat(0.0),
      m_maxLat(0.0),
      m_heightAxis(Height)
{
    ui.setupUi(this);
    
    setMinimumHeight(150);
    
    connectSliders();
}

DimensionSliders::~DimensionSliders()
{
}

MainWindow *DimensionSliders::mainWindow() 
{
    return m_mainWindow;
}

void DimensionSliders::setMainWindow(MainWindow *mainWindow)
{
    if(m_mainWindow) {
        disconnect(m_mainWindow, 0, this, 0);
    }
    
    m_mainWindow = mainWindow;
    
    connect(m_mainWindow, SIGNAL(coverageIdChanged(const QString&)),
            this, SLOT(setCoverageId(const QString&)));
    connect(m_mainWindow, SIGNAL(viewModeChanged(MainWindow::ViewMode)),
            this, SLOT(updateShownSliders(MainWindow::ViewMode)));
    
    connect(m_mainWindow, SIGNAL(timeChanged(const QDateTime&)),
            this, SLOT(setTime(const QDateTime&)));
    connect(m_mainWindow, SIGNAL(lonChanged(qreal)),
            this, SLOT(setLon(qreal)));
    connect(m_mainWindow, SIGNAL(latChanged(qreal)),
            this, SLOT(setLat(qreal)));
    connect(m_mainWindow, SIGNAL(heightChanged(qreal)),
            this, SLOT(setHeight(qreal)));
    
    connect(this, SIGNAL(timeChanged(const QDateTime&)),
            m_mainWindow, SLOT(setTime(const QDateTime&)));
    connect(this, SIGNAL(lonChanged(qreal)),
            m_mainWindow, SLOT(setLon(qreal)));
    connect(this, SIGNAL(latChanged(qreal)),
            m_mainWindow, SLOT(setLat(qreal)));
    connect(this, SIGNAL(heightChanged(qreal)),
            m_mainWindow, SLOT(setHeight(qreal)));
    
    updateShownSliders(mainWindow->viewMode());
    setCoverageId(mainWindow->coverageId());
    updateLimits();
}

void DimensionSliders::updateLimits()
{
    m_valueCountTime = 0;
    m_valueCountLon = 0;
    m_valueCountLat = 0;
    
    foreach(CoordinateAxis axis, coordinateAxes()) {
        switch(axis.dimension()) {
            case Time:
                m_minTime = axis.lowerLimit().toDateTime();
                m_maxTime = axis.upperLimit().toDateTime();
                m_valueCountTime = axis.valueCount();
                break;
            case Lon:
                m_minLon = axis.lowerLimit().toDouble();
                m_maxLon = axis.upperLimit().toDouble();
                m_valueCountLon = axis.valueCount();
                break;
            case Lat:
                m_minLat = axis.lowerLimit().toDouble();
                m_maxLat = axis.upperLimit().toDouble();
                m_valueCountLat = axis.valueCount();
                break;
            case Height:
                m_heightAxis = axis;
                break;
        }
    }
    
    disconnectSliders();
    ui.timeSlider->setMinimum(0);
    ui.timeSlider->setMaximum(m_valueCountTime - 1);
    ui.lonSlider->setMinimum(0);
    ui.lonSlider->setMaximum(m_valueCountLon - 1);
    ui.latSlider->setMinimum(0);
    ui.latSlider->setMaximum(m_valueCountLat - 1);
    ui.heightSlider->setMinimum(0);
    ui.heightSlider->setMaximum(m_heightAxis.valueCount() - 1);
    connectSliders();
    
    setTime(m_mainWindow->time());
    setLon(m_mainWindow->lon());
    setLat(m_mainWindow->lat());
    setHeight(m_mainWindow->height());
}

void DimensionSliders::updateShownSliders(MainWindow::ViewMode viewMode)
{
    // Time
    switch(viewMode) {
        case MainWindow::LonLat:
        case MainWindow::LonHeight:
        case MainWindow::LatHeight:
            showTimeSlider(true);
            break;
        default:
            showTimeSlider(false);
    }
    
    // Lon
    switch(viewMode) {
        case MainWindow::TimeLat:
        case MainWindow::TimeHeight:
        case MainWindow::LatHeight:
            showLonSlider(true);
            break;
        default:
            showLonSlider(false);
    }
    
    // Lat
    switch(viewMode) {
        case MainWindow::TimeLon:
        case MainWindow::TimeHeight:
        case MainWindow::LonHeight:
            showLatSlider(true);
            break;
        default:
            showLatSlider(false);
    }
    
    // Height
    switch(viewMode) {
        case MainWindow::TimeLon:
        case MainWindow::TimeLat:
        case MainWindow::LonLat:
            showHeightSlider(true);
            break;
        default:
            showHeightSlider(false);
    }
}

void DimensionSliders::setTime(const QDateTime& time)
{
    uint minTimeSeconds = m_minTime.toTime_t();
    uint maxTimeSeconds = m_maxTime.toTime_t();
    uint timeSeconds = time.toTime_t();
    
    if(minTimeSeconds == maxTimeSeconds) {
        return;
    }
    
    qreal timeScale = (qreal) m_valueCountTime / (qreal) (maxTimeSeconds - minTimeSeconds);
    int timeIndex = std::floor((timeSeconds - minTimeSeconds) * timeScale);
    
    if(timeIndex < 0 || timeIndex == ui.timeSlider->value()) {
        return;
    }
    
    disconnectSliders();
    ui.timeSlider->setValue(timeIndex);
    connectSliders();
    
    emit timeChanged(time);
}

void DimensionSliders::setLon(qreal lon)
{
    if(m_minLon == m_maxLon) {
        return;
    }
    
    qreal lonScale = (qreal) m_valueCountLon / (m_maxLon - m_minLon);
    int lonIndex = std::floor((lon - m_minLon) * lonScale);
    
    if(lonIndex < 0 || lonIndex == ui.lonSlider->value()) {
        return;
    }
    
    disconnectSliders();
    ui.lonSlider->setValue(lonIndex);
    connectSliders();

    emit lonChanged(lon);
}

void DimensionSliders::setLat(qreal lat)
{
    if(m_minLon == m_maxLat) {
        return;
    }
    
    qreal latScale = (qreal) m_valueCountLat / (m_maxLat - m_minLat);
    int latIndex = std::floor((lat - m_minLat) * latScale);
    
    if(latIndex < 0 || latIndex == ui.latSlider->value()) {
        return;
    }
    
    disconnectSliders();
    ui.latSlider->setValue(latIndex);
    connectSliders();
    
    emit latChanged(lat);
}

void DimensionSliders::setHeight(qreal height)
{
    int heightIndex = 0;
    QList<QVariant> heightValues = m_heightAxis.values();
    int valueCountHeight = m_heightAxis.valueCount();
    double minHeight = m_heightAxis.lowerLimit().toDouble();
    double maxHeight = m_heightAxis.upperLimit().toDouble();
    
    if(height < minHeight || height >= maxHeight) {
        qDebug() << "Height to small/high";
        heightIndex = valueCountHeight - 1;
        
        QList<QVariant> heightValues = m_heightAxis.values();
        int heightValuesSize = heightValues.size();
        if(heightValuesSize >= 2) {
            height = (heightValues.at(heightValuesSize - 2).toDouble() + heightValues.at(heightValuesSize - 1).toDouble()) / 2;
            qDebug() << "New height =" << height;
        }
    }
    else if(!heightValues.isEmpty()) {
        for(int i = 0; i <= valueCountHeight; ++i) {
            QVariant heightValue = heightValues[i];
            double heightDouble = heightValue.toDouble();
            
            if(heightDouble > height) {
                heightIndex = i - 1;
                break;
            }
        }
    }
    else {
        int valueCountHeight = m_heightAxis.valueCount();
        
        if(minHeight == maxHeight) {
            return;
        }
        
        qreal heightScale = (qreal) valueCountHeight / (maxHeight - minHeight);
        heightIndex = std::floor((height - minHeight) * heightScale);
        
        if(heightIndex < 0 || heightIndex == ui.heightSlider->value()) {
            return;
        }
    }
    
    if(heightIndex != ui.heightSlider->value()) {
        ui.heightSlider->setValue(heightIndex);
    }
        
    emit heightChanged(height);
}

void DimensionSliders::handleChangedTimeValue(int timeValue)
{
    uint minTimeSeconds = m_minTime.toTime_t();
    uint maxTimeSeconds = m_maxTime.toTime_t();
    
    if(minTimeSeconds == maxTimeSeconds || m_valueCountTime == 0) {
        return;
    }
    
    qreal timeScale = (qreal) (maxTimeSeconds - minTimeSeconds) / (qreal) m_valueCountTime;
    
    QDateTime time;
    time.setTime_t(minTimeSeconds + timeScale * timeValue);
    
    emit timeChanged(time);
}

void DimensionSliders::handleChangedLonValue(int lonValue)
{
    if(m_minLon == m_maxLon || m_valueCountLon == 0) {
        return;
    }
    
    qreal lonScale = (m_maxLon - m_minLon) / (qreal) m_valueCountLon;
    
    emit lonChanged(m_minLon + lonScale * (lonValue + 0.5));
}

void DimensionSliders::handleChangedLatValue(int latValue)
{
    if(m_minLat == m_maxLat || m_valueCountLat == 0) {
        return;
    }
    
    qreal latScale = (m_maxLat - m_minLat) / (qreal) m_valueCountLat;
    
    emit latChanged(m_minLat + latScale * (latValue + 0.5));
}

void DimensionSliders::handleChangedHeightValue(int heightIndex)
{
    double minHeight = m_heightAxis.lowerLimit().toDouble();
    double maxHeight = m_heightAxis.upperLimit().toDouble();
    double valueCountHeight = m_heightAxis.valueCount();
    if(minHeight == maxHeight || valueCountHeight == 0) {
        return;
    }
    
    QList<QVariant> heightValues = m_heightAxis.values();
    
    if(!heightValues.isEmpty()) {
        emit heightChanged((heightValues[heightIndex].toDouble() + heightValues[heightIndex + 1].toDouble()) / 2);
    }
    else {
        qreal heightScale = (maxHeight - minHeight) / (qreal) valueCountHeight;
        
        emit heightChanged(minHeight + heightScale * (heightIndex + 0.5));
    }
}

void DimensionSliders::connectSliders()
{
    connect(ui.timeSlider, SIGNAL(valueChanged(int)),
            this, SLOT(handleChangedTimeValue(int)));
    connect(ui.lonSlider, SIGNAL(valueChanged(int)),
            this, SLOT(handleChangedLonValue(int)));
    connect(ui.latSlider, SIGNAL(valueChanged(int)),
            this, SLOT(handleChangedLatValue(int)));
    connect(ui.heightSlider, SIGNAL(valueChanged(int)),
            this, SLOT(handleChangedHeightValue(int)));
}

void DimensionSliders::disconnectSliders()
{
    if(!ui.timeSlider) {
        return;
    }
    
    disconnect(ui.timeSlider, SIGNAL(valueChanged(int)),
               this, SLOT(handleChangedTimeValue(int)));
    disconnect(ui.lonSlider, SIGNAL(valueChanged(int)),
               this, SLOT(handleChangedLonValue(int)));
    disconnect(ui.latSlider, SIGNAL(valueChanged(int)),
               this, SLOT(handleChangedLatValue(int)));
    disconnect(ui.heightSlider, SIGNAL(valueChanged(int)),
               this, SLOT(handleChangedHeightValue(int)));
}

void DimensionSliders::setCoverageId(const QString& coverageId)
{
    m_coverageId = coverageId;
    updateLimits();
    updateShownSliders(m_mainWindow->viewMode());
}

void DimensionSliders::showTimeSlider(bool show)
{
    if(show && axisAvailable(Time)) {
        ui.timeSlider->show();
        ui.timeLabel->show();
    }
    else {
        ui.timeSlider->hide();
        ui.timeLabel->hide();
    }
}

void DimensionSliders::showLonSlider(bool show)
{
    if(show && axisAvailable(Lon)) {
        ui.lonSlider->show();
        ui.lonLabel->show();
    }
    else {
        ui.lonSlider->hide();
        ui.lonLabel->hide();
    }
}

void DimensionSliders::showLatSlider(bool show)
{
    if(show && axisAvailable(Lat)) {
        ui.latSlider->show();
        ui.latLabel->show();
    }
    else {
        ui.latSlider->hide();
        ui.latLabel->hide();
    }
}

void DimensionSliders::showHeightSlider(bool show)
{
    if(show && axisAvailable(Height)) {
        ui.heightSlider->show();
        ui.heightLabel->show();
    }
    else {
        ui.heightSlider->hide();
        ui.heightLabel->hide();
    }
}

QList<CoordinateAxis> DimensionSliders::coordinateAxes() const
{
    QList<Coverage> coverages = m_mainWindow->coverages();
    
    foreach(Coverage coverage, coverages) {
        if(coverage.coverageId() == m_coverageId) {
            return coverage.coordinateAxes();
        }
    }
    
    return QList<CoordinateAxis>();
}

bool DimensionSliders::axisAvailable(Dimension dimension) const
{
    foreach(CoordinateAxis axis, coordinateAxes()) {
        if(axis.dimension() == dimension) {
            return true;
        }
    }
    
    return false;
}

#include "DimensionSliders.moc"
