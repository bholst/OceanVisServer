//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtGui/QMainWindow>

// Project
#include "CoverageComboBox.h"

class QNetworkAccessManager;
class QNetworkReply;
class GetCoverage;
class QDateTime;
class MapWidget;
class Coverage;
class DimensionSliders;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();
    
    const GetCoverage *request() const;
    
    enum ViewMode {
        TimeLon,
        TimeLat,
        TimeHeight,
        LonLat,
        LonHeight,
        LatHeight
    };
    
    QList<Coverage> coverages() const;
    
    QString coverageId() const;
    QDateTime time() const;
    qreal lon() const;
    qreal lat() const;
    qreal height() const;
    ViewMode viewMode() const;
    
signals:
    void requestChanged();
    void coverageIdChanged(const QString& coverageId);
    void timeChanged(const QDateTime& time);
    void lonChanged(qreal lon);
    void latChanged(qreal lat);
    void heightChanged(qreal height);
    void viewModeChanged(MainWindow::ViewMode viewMode);
    void urlChanged(const QString& url);
    void requestStringChanged(const QString&);
    void coveragesChanged(const QList<Coverage>& coverages);
    
public slots:
    void setCoverageId(const QString& coverageId);
    void setTime(const QDateTime& time);
    void setLon(qreal lon);
    void setLat(qreal lat);
    void setHeight(qreal height);
    void setViewMode(ViewMode viewMode);
    void setUrl(const QString& url);
    
protected:
    void closeEvent(QCloseEvent *event);
    
private slots:
    void updateRequestString();
    void parseCoverages(QNetworkReply *reply);

private:
    void updateRequestSubsets();
    
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    
    MapWidget *m_mapWidget;
    CoverageComboBox *m_coverageComboBox;
    DimensionSliders *m_dimensionSliders;
    
    GetCoverage *m_request;
    QDateTime m_time;
    qreal m_lon;
    qreal m_lat;
    qreal m_height;
    ViewMode m_viewMode;
    QString m_url;
    QNetworkAccessManager *m_manager;
    QList<Coverage> m_coverages;
};

#endif
