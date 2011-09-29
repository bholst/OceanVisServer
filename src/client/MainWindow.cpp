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
#include <QtGui/QStatusBar>
#include <QtGui/QDockWidget>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>

// Project
#include "GetCoverage.h"
#include "DimensionSubset.h"
#include "DimensionSlice.h"
#include "MapWidget.h"
#include "CoveragesParser.h"
#include "Coverage.h"
#include "DimensionSliders.h"
#include "CoverageComboBox.h"
#include "ViewModeComboBox.h"
#include "ColorMapWidget.h"
#include "ColorMap.h"

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
    
    //// GUI
    // MapWidget
    m_mapWidget = new MapWidget();
    setCentralWidget(m_mapWidget);
    connect(this, SIGNAL(requestStringChanged(const QString&)),
            m_mapWidget, SLOT(setUrl(const QString&)));
    connect(this, SIGNAL(xAxisChanged(const QString&, const QString&, const QString&)),
            m_mapWidget, SLOT(setXAxis(const QString&, const QString&, const QString&)));
    connect(this, SIGNAL(yAxisChanged(const QString&, const QString&, const QString&)),
            m_mapWidget, SLOT(setYAxis(const QString&, const QString&, const QString&)));
    
    
    // CoverageComboBox & ViewModeComboBox & FitWindow
    QDockWidget *selectionDockWidget = new QDockWidget(this);
    QWidget *selectionWidget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(selectionWidget);
    selectionWidget->setLayout(layout);
    
    m_coverageComboBox = new CoverageComboBox(selectionWidget);
    layout->addWidget(m_coverageComboBox);
    connect(m_coverageComboBox, SIGNAL(currentCoverageChanged(const QString&)),
            this, SLOT(setCoverageId(const QString&)));
    connect(this, SIGNAL(coveragesChanged(const QList<Coverage>&)),
            m_coverageComboBox, SLOT(setCoverages(const QList<Coverage>&)));
    
    m_viewModeComboBox = new ViewModeComboBox(selectionWidget);
    layout->addWidget(m_viewModeComboBox);
    connect(m_viewModeComboBox, SIGNAL(currentViewModeChanged(MainWindow::ViewMode)),
            this, SLOT(setViewMode(MainWindow::ViewMode)));
    connect(this, SIGNAL(viewModeChanged(MainWindow::ViewMode)),
            m_viewModeComboBox, SLOT(setViewMode(MainWindow::ViewMode)));
    m_viewModeComboBox->setViewMode(viewMode());
    
    m_fitWindowCheckBox = new QCheckBox(selectionWidget);
    m_fitWindowCheckBox->setText("Fit Window");
    layout->addWidget(m_fitWindowCheckBox);
    connect(m_fitWindowCheckBox, SIGNAL(toggled(bool)),
            m_mapWidget, SLOT(setFitWindow(bool)));
    
    selectionDockWidget->setWidget(selectionWidget);
    addDockWidget(Qt::TopDockWidgetArea, selectionDockWidget);
    
    // CoverageComboBox
    
    // DimensionSliders
    QDockWidget *dimensionDockWidget = new QDockWidget(this);
    m_dimensionSliders = new DimensionSliders(dimensionDockWidget);
    dimensionDockWidget->setWidget(m_dimensionSliders);
    addDockWidget(Qt::LeftDockWidgetArea, dimensionDockWidget);
    m_dimensionSliders->setMainWindow(this);
    
    // ColorMapWidget
    QDockWidget *colorMapDockWidget = new QDockWidget(this);
    m_colorMapWidget = new ColorMapWidget(colorMapDockWidget);
    colorMapDockWidget->setWidget(m_colorMapWidget);
    addDockWidget(Qt::LeftDockWidgetArea, colorMapDockWidget);
    connect(this, SIGNAL(colorMapChanged(const ColorMap&)),
            m_colorMapWidget, SLOT(setColorMap(const ColorMap&)));
    connect(this, SIGNAL(minValueChanged(double)),
            m_colorMapWidget, SLOT(setMinValue(double)));
    connect(this, SIGNAL(maxValueChanged(double)),
            m_colorMapWidget, SLOT(setMaxValue(double)));
    
    // StatusBar
    m_statusLabel = new QLabel(statusBar());
    statusBar()->addWidget(m_statusLabel);
    connect(this, SIGNAL(timeChanged(const QDateTime&)),
            this, SLOT(updateStatusBar()));
    connect(this, SIGNAL(lonChanged(qreal)),
            this, SLOT(updateStatusBar()));
    connect(this, SIGNAL(latChanged(qreal)),
            this, SLOT(updateStatusBar()));
    connect(this, SIGNAL(heightChanged(qreal)),
            this, SLOT(updateStatusBar()));
    connect(this, SIGNAL(viewModeChanged(MainWindow::ViewMode)),
            this, SLOT(updateStatusBar()));
    updateStatusBar();
    
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();
    
    connect(this, SIGNAL(requestChanged()),
            this, SLOT(updateRequestString()));
    connect(m_manager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(parseCoverages(QNetworkReply *)));
    connect(this, SIGNAL(coverageIdChanged(const QString&)),
            this, SLOT(emitColorMapChanged(const QString&)));
    connect(this, SIGNAL(coverageIdChanged(const QString&)),
            this, SLOT(emitAxesChanged()));
    connect(this, SIGNAL(viewModeChanged(MainWindow::ViewMode)),
            this, SLOT(emitAxesChanged()));
    emitAxesChanged();
    connect(this, SIGNAL(coverageIdChanged(const QString&)),
            this, SLOT(emitValuesChanged()));
    emitValuesChanged();
            
    setMinimumWidth(640);
    setMinimumHeight(480);
}

MainWindow::~MainWindow()
{
}

const GetCoverage *MainWindow::request() const
{
    return m_request;
}

QList<Coverage> MainWindow::coverages() const
{
    return m_coverages;
}

QString MainWindow::coverageId() const
{
    return m_request->coverageId();
}

QDateTime MainWindow::time() const
{
    return m_time;
}

qreal MainWindow::lon() const
{
    return m_lon;
}

qreal MainWindow::lat() const
{
    return m_lat;
}

qreal MainWindow::height() const
{
    return m_height;
}

MainWindow::ViewMode MainWindow::viewMode() const
{
    return m_viewMode;
}

void MainWindow::setCoverageId(const QString& coverageId)
{
    m_request->setCoverageId(coverageId);
    
    emit coverageIdChanged(coverageId);
    emit requestChanged();
}

void MainWindow::setTime(const QDateTime& time)
{
    if(time == m_time)
        return;
    
    m_time = time;
    
    updateRequestSubsets();
    emit timeChanged(time);
}

void MainWindow::setLon(qreal lon)
{
    if(lon == m_lon)
        return;

    m_lon = lon;
    
    updateRequestSubsets();
    emit lonChanged(lon);
}

void MainWindow::setLat(qreal lat)
{
    if(lat == m_lat)
        return;

    m_lat = lat;
    
    updateRequestSubsets();
    emit latChanged(lat);
}

void MainWindow::setHeight(qreal height)
{
    if(height == m_height)
        return;

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

void MainWindow::updateStatusBar()
{
    QString statusString;
    
    switch(m_viewMode) {
        case TimeLon:
            statusString = tr("Lat: %1; Height: %2");
            statusString = statusString.arg(QLocale::system().toString(m_lat));
            statusString = statusString.arg(QLocale::system().toString(m_height));
            break;
        case TimeLat:
            statusString = tr("Lon: %1; Height: %2");
            statusString = statusString.arg(QLocale::system().toString(m_lon));
            statusString = statusString.arg(QLocale::system().toString(m_height));
            break;
        case TimeHeight:
            statusString = tr("Lon: %1; Lat: %2");
            statusString = statusString.arg(QLocale::system().toString(m_lon));
            statusString = statusString.arg(QLocale::system().toString(m_lat));
            break;
        case LonLat:
            statusString = tr("Time: %1; Height: %2");
            statusString = statusString.arg(QLocale::system().toString(m_time, QLocale::ShortFormat));
            statusString = statusString.arg(QLocale::system().toString(m_height));
            break;
        case LonHeight:
            statusString = tr("Time: %1; Lat: %2");
            statusString = statusString.arg(QLocale::system().toString(m_time, QLocale::ShortFormat));
            statusString = statusString.arg(QLocale::system().toString(m_lat));
            break;
        case LatHeight:
            statusString = tr("Time: %1; Lon: %2");
            statusString = statusString.arg(QLocale::system().toString(m_time, QLocale::ShortFormat));
            statusString = statusString.arg(QLocale::system().toString(m_lon));
            break;
    }
    
    m_statusLabel->setText(statusString);
}

void MainWindow::emitColorMapChanged(const QString& coverageId)
{
    foreach(Coverage coverage, m_coverages) {
        if(coverage.coverageId() == coverageId) {
            emit colorMapChanged(coverage.colorMap());
            break;
        }
    }
}

void MainWindow::emitAxesChanged()
{
    CoordinateAxis timeAxis(Time);
    CoordinateAxis lonAxis(Lon);
    CoordinateAxis latAxis(Lat);
    CoordinateAxis heightAxis(Height);
    
    foreach(Coverage coverage, m_coverages) {
        if(coverage.coverageId() == m_request->coverageId()) {
            foreach(CoordinateAxis axis, coverage.coordinateAxes()) {
                switch(axis.dimension()) {
                    case Time:
                        timeAxis = axis;
                        break;
                    case Lon:
                        lonAxis = axis;
                        break;
                    case Lat:
                        latAxis = axis;
                        break;
                    case Height:
                        heightAxis = axis;
                        break;
                }
            }
        }
    }
    
    CoordinateAxis xAxis(Lon);
    CoordinateAxis yAxis(Lat);
    
    switch(m_viewMode) {
        case TimeLon:
            xAxis = timeAxis;
            yAxis = lonAxis;
            break;
        case TimeLat:
            xAxis = timeAxis;
            yAxis = latAxis;
            break;
        case TimeHeight:
            xAxis = timeAxis;
            yAxis = heightAxis;
            break;
        case LonLat:
            xAxis = lonAxis;
            yAxis = latAxis;
            break;
        case LonHeight:
            xAxis = lonAxis;
            yAxis = heightAxis;
            break;
        case LatHeight:
            xAxis = latAxis;
            yAxis = heightAxis;
            break;
    }
    
    emit xAxisChanged(dimensionToString(xAxis.dimension()),
                      variantToString(xAxis.dimension(), xAxis.lowerLimit()).replace('T', ' '),
                      variantToString(xAxis.dimension(), xAxis.upperLimit()).replace('T', ' '));
    emit yAxisChanged(dimensionToString(yAxis.dimension()),
                      variantToString(yAxis.dimension(), yAxis.lowerLimit()),
                      variantToString(yAxis.dimension(), yAxis.upperLimit()));
}

void MainWindow::emitValuesChanged()
{
    foreach(Coverage coverage, m_coverages) {
        if(coverage.coverageId() == m_request->coverageId()) {
            emit minValueChanged(coverage.minValue());
            emit maxValueChanged(coverage.maxValue());
            break;
        }
    }
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
