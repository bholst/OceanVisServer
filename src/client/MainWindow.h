//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtGui/QMainWindow>

class QLabel;
class QNetworkAccessManager;
class QNetworkReply;
class GetCoverage;
class QDateTime;
class MapWidget;
class Coverage;
class DimensionSliders;
class ViewModeComboBox;
class CoverageComboBox;
class ColorMapWidget;
class ColorMap;

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
    void colorMapChanged(const ColorMap& colorMap);
    void xAxisChanged(const QString& name, const QString& min, const QString& max);
    void yAxisChanged(const QString& name, const QString& min, const QString& max);
    void minValueChanged(double minValue);
    void maxValueChanged(double maxValue);
    
public slots:
    void setCoverageId(const QString& coverageId);
    void setTime(const QDateTime& time);
    void setLon(qreal lon);
    void setLat(qreal lat);
    void setHeight(qreal height);
    void setViewMode(MainWindow::ViewMode viewMode);
    void setUrl(const QString& url);
    
protected:
    void closeEvent(QCloseEvent *event);
    
private slots:
    void updateRequestString();
    void parseCoverages(QNetworkReply *reply);
    void updateStatusBar();
    void emitColorMapChanged(const QString& coverageId);
    void emitAxesChanged();
    void emitValuesChanged();

private:
    void updateRequestSubsets();
    
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    
    MapWidget *m_mapWidget;
    ViewModeComboBox *m_viewModeComboBox;
    CoverageComboBox *m_coverageComboBox;
    DimensionSliders *m_dimensionSliders;
    ColorMapWidget *m_colorMapWidget;
    QLabel *m_statusLabel;
    
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
