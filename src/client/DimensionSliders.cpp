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
      m_valueCountHeight(0),
      m_minLon(0.0),
      m_maxLon(0.0),
      m_minLat(0.0),
      m_maxLat(0.0),
      m_minHeight(0.0),
      m_maxHeight(0.0)
{
    ui.setupUi(this);
    
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
            this, SLOT(updateLimits(const QString&)));
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
    updateLimits(mainWindow->coverageId());
}

void DimensionSliders::updateLimits(const QString& coverageId)
{
    QList<Coverage> coverages = m_mainWindow->coverages();
    
    m_valueCountTime = 0;
    m_valueCountLon = 0;
    m_valueCountLat = 0;
    m_valueCountHeight = 0;
    
    foreach(Coverage coverage, coverages) {
        if(coverage.coverageId() == coverageId) {
            foreach(CoordinateAxis axis, coverage.coordinateAxes()) {
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
                        m_minHeight = axis.lowerLimit().toDouble();
                        m_maxHeight = axis.upperLimit().toDouble();
                        m_valueCountHeight = axis.valueCount();
                        break;
                }
            }
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
    ui.heightSlider->setMaximum(m_valueCountHeight - 1);
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
            ui.timeSlider->show();
            ui.timeLabel->show();
            break;
        default:
            ui.timeSlider->hide();
            ui.timeLabel->hide();
    }
    
    // Lon
    switch(viewMode) {
        case MainWindow::TimeLat:
        case MainWindow::TimeHeight:
        case MainWindow::LatHeight:
            ui.lonSlider->show();
            ui.lonLabel->show();
            break;
        default:
            ui.lonSlider->hide();
            ui.lonLabel->hide();
    }
    
    // Lat
    switch(viewMode) {
        case MainWindow::TimeLon:
        case MainWindow::TimeHeight:
        case MainWindow::LonHeight:
            ui.latSlider->show();
            ui.latLabel->show();
            break;
        default:
            ui.latSlider->hide();
            ui.latLabel->hide();
    }
    
    // Height
    switch(viewMode) {
        case MainWindow::TimeLon:
        case MainWindow::TimeLat:
        case MainWindow::LonLat:
            ui.heightSlider->show();
            ui.heightLabel->show();
            break;
        default:
            ui.heightSlider->hide();
            ui.heightLabel->hide();
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
    if(m_minHeight == m_maxHeight) {
        return;
    }
    
    qreal heightScale = (qreal) m_valueCountHeight / (m_maxHeight - m_minHeight);
    int heightIndex = std::floor((height - m_minHeight) * heightScale);
    
    if(heightIndex < 0 || heightIndex == ui.heightSlider->value()) {
        return;
    }
    
    disconnectSliders();
    ui.heightSlider->setValue(heightIndex);
    connectSliders();
    
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
    
    emit lonChanged(m_minLon + lonScale * lonValue);
}

void DimensionSliders::handleChangedLatValue(int latValue)
{
    if(m_minLat == m_maxLat || m_valueCountLat == 0) {
        return;
    }
    
    qreal latScale = (m_maxLat - m_minLat) / (qreal) m_valueCountLat;
    
    emit latChanged(m_minLat + latScale * latValue);
}

void DimensionSliders::handleChangedHeightValue(int heightValue)
{
    if(m_minHeight == m_maxHeight || m_valueCountHeight == 0) {
        return;
    }
    
    qreal heightScale = (m_maxHeight - m_minHeight) / (qreal) m_valueCountHeight;
    
    emit heightChanged(m_minHeight + heightScale * heightValue);
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

#include "DimensionSliders.moc"
