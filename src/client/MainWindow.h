//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QtCore/QDateTime>
#include <QtGui/QMainWindow>

class GetCoverage;
class QDateTime;
class MapWidget;

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
    
signals:
    void requestChanged();
    void coverageIdChanged(const QString& coverageId);
    void timeChanged(const QDateTime& time);
    void lonChanged(qreal lon);
    void latChanged(qreal lat);
    void heightChanged(qreal height);
    void viewModeChanged(ViewMode viewMode);
    void urlChanged(const QString& url);
    void requestStringChanged(const QString&);
    
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

private:
    void updateRequestSubsets();
    
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    
    MapWidget *m_mapWidget;
    
    GetCoverage *m_request;
    QDateTime m_time;
    qreal m_lon;
    qreal m_lat;
    qreal m_height;
    ViewMode m_viewMode;
    QString m_url;
};

#endif