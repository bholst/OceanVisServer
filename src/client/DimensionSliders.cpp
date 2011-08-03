//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <cmath>

// Qt
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
    disconnect(m_mainWindow, 0, this, 0);
    
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
    
    ui.timeSlider->setMinimum(0);
    ui.timeSlider->setMaximum(m_valueCountTime - 1);
    ui.lonSlider->setMinimum(0);
    ui.lonSlider->setMaximum(m_valueCountLon - 1);
    ui.latSlider->setMinimum(0);
    ui.latSlider->setMaximum(m_valueCountLat - 1);
    ui.heightSlider->setMinimum(0);
    ui.heightSlider->setMaximum(m_valueCountHeight - 1);
    
    setTime(m_mainWindow->time());
    setLon(m_mainWindow->lon());
    setLat(m_mainWindow->lat());
    setHeight(m_mainWindow->height());
}

void DimensionSliders::updateShownSliders(MainWindow::ViewMode viewMode)
{
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
    
    ui.timeSlider->setValue(timeIndex);
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
    
    ui.lonSlider->setValue(lonIndex);
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
    
    ui.latSlider->setValue(latIndex);
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
    
    ui.heightSlider->setValue(heightIndex);
    emit heightChanged(height);
}

#include "DimensionSliders.moc"
