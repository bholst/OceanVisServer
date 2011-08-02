//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>
#include <QtCore/QDateTime>

// Project
#include "GetCoverage.h"
#include "DimensionSubset.h"
#include "DimensionSlice.h"
#include "MapWidget.h"

// Self
#include "MainWindow.h"

MainWindow::MainWindow()
    : m_request(new GetCoverage(RequestBase::OVP)),
      m_time(QDateTime(QDate(2011, 8, 2), QTime(13, 27))),
      m_lon(0.0),
      m_lat(0.0),
      m_height(0.0),
      m_viewMode(MainWindow::LonLat)
{
    m_request->setVersion("1.0.0");
    updateRequestSubsets();
    m_mapWidget = new MapWidget();
    setCentralWidget(m_mapWidget);
    
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();
    
    connect(this, SIGNAL(requestChanged()),
            this, SLOT(updateRequestString()));
    connect(this, SIGNAL(requestStringChanged(const QString&)),
            m_mapWidget, SLOT(setUrl(const QString&)));
            
}

MainWindow::~MainWindow()
{
}

const GetCoverage *MainWindow::request() const
{
    return m_request;
}

void MainWindow::setCoverageId(const QString& coverageId)
{
    m_request->setCoverageId(coverageId);
    
    emit coverageIdChanged(coverageId);
    emit requestChanged();
}

void MainWindow::setTime(const QDateTime& time)
{
    m_time = time;
    
    updateRequestSubsets();
    emit timeChanged(time);
}

void MainWindow::setLon(qreal lon)
{
    m_lon = lon;
    
    updateRequestSubsets();
    emit lonChanged(lon);
}

void MainWindow::setLat(qreal lat)
{
    m_lat = lat;
    
    updateRequestSubsets();
    emit latChanged(lat);
}

void MainWindow::setHeight(qreal height)
{
    m_height = height;
    
    updateRequestSubsets();
    emit heightChanged(height);
}

void MainWindow::setViewMode(ViewMode viewMode)
{
    m_viewMode = viewMode;
    
    updateRequestSubsets();
    emit viewModeChanged(viewMode);
}

void MainWindow::setUrl(const QString& url)
{
    m_url = url;
    
    emit urlChanged(url);
    emit requestChanged();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
}

void MainWindow::updateRequestString()
{
    qDebug() << "Updating request string.";
    qDebug() << m_request->toRequestString(m_url);
    emit requestStringChanged(m_request->toRequestString(m_url));
}

void MainWindow::updateRequestSubsets()
{
    QList<DimensionSubset *> subsets;
    
    // Time
    switch(m_viewMode) {
        case LonLat:
        case LonHeight:
        case LatHeight:
            DimensionSlice *time = new DimensionSlice(Time);
            time->setSlicePoint(m_time);
            subsets.append(time);
            break;
    }
    
    // Lon
    switch(m_viewMode) {
        case TimeLat:
        case TimeHeight:
        case LatHeight:
            DimensionSlice *lon = new DimensionSlice(Lon);
            lon->setSlicePoint(m_lon);
            subsets.append(lon);
            break;
    }
    
    // Lat
    switch(m_viewMode) {
        case TimeLon:
        case TimeHeight:
        case LonHeight:
            DimensionSlice *lat = new DimensionSlice(Lat);
            lat->setSlicePoint(m_lat);
            subsets.append(lat);
            break;
    }
    
    // Height
    switch(m_viewMode) {
        case TimeLon:
        case TimeLat:
        case LonLat:
            DimensionSlice *height = new DimensionSlice(Height);
            height->setSlicePoint(m_height);
            subsets.append(height);
            break;
    }
        
    m_request->setDimensionSubsets(subsets);
    emit requestChanged();
}

void MainWindow::createActions()
{
}

void MainWindow::createMenus()
{
}

void MainWindow::createToolBars()
{
}

void MainWindow::createStatusBar()
{
}

void MainWindow::readSettings()
{
}

void MainWindow::writeSettings()
{
}

#include "MainWindow.moc"
