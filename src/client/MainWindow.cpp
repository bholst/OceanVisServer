//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QUrl>
#include <QtGui/QDockWidget>

// Project
#include "GetCoverage.h"
#include "DimensionSubset.h"
#include "DimensionSlice.h"
#include "MapWidget.h"
#include "CoveragesParser.h"
#include "Coverage.h"

// Self
#include "MainWindow.h"

MainWindow::MainWindow()
    : m_request(new GetCoverage(RequestBase::OVP)),
      m_time(QDateTime(QDate(2011, 8, 2), QTime(13, 27))),
      m_lon(0.0),
      m_lat(0.0),
      m_height(0.0),
      m_viewMode(MainWindow::LonLat),
      m_manager(new QNetworkAccessManager(this))
{
    m_request->setVersion("1.0.0");
    updateRequestSubsets();
    
    // GUI
    m_mapWidget = new MapWidget();
    setCentralWidget(m_mapWidget);
    connect(this, SIGNAL(requestStringChanged(const QString&)),
            m_mapWidget, SLOT(setUrl(const QString&)));
    
    QDockWidget *coverageDockWidget = new QDockWidget(this);
    m_coverageComboBox = new CoverageComboBox(coverageDockWidget);
    coverageDockWidget->setWidget(m_coverageComboBox);
    connect(m_coverageComboBox, SIGNAL(currentCoverageChanged(const QString&)),
            this, SLOT(setCoverageId(const QString&)));
    connect(this, SIGNAL(coveragesChanged(const QList<Coverage>&)),
            m_coverageComboBox, SLOT(setCoverages(const QList<Coverage>&)));
    addDockWidget(Qt::TopDockWidgetArea, coverageDockWidget);
    
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();
    
    connect(this, SIGNAL(requestChanged()),
            this, SLOT(updateRequestString()));
    connect(m_manager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(parseCoverages(QNetworkReply *)));
            
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

    QString requestUrl(m_url);
    requestUrl += "?SERVICE=ovp&REQUEST=DescribeCoverages&VERSION=1.0.0";
    m_manager->get(QNetworkRequest(QUrl(requestUrl)));
    m_coverages.clear();
    
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

void MainWindow::parseCoverages(QNetworkReply *reply)
{
    if(reply->error()) {
        QString url = reply->url().toString();
        fprintf(stderr, "Download of %s failed: %s\n",
                url.toStdString().c_str(),
                qPrintable(reply->errorString()));
    }
    else {
        qDebug() << "Reading coverages.";
        CoveragesParser parser;
        parser.setDevice(reply);
        parser.read();
        m_coverages = parser.coverages();
        emit coveragesChanged(m_coverages);
    }

    reply->deleteLater();
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
