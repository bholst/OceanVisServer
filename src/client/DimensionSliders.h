//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef DIMENSIONSLIDERS_H
#define DIMENSIONSLIDERS_H

// Qt
#include <QtGui/QWidget>

// Project
#include "MainWindow.h"

// Self
#include "ui_DimensionSliders.h"

class DimensionSliders : public QWidget {
    Q_OBJECT
public:
    DimensionSliders(QWidget *parent = 0);
    ~DimensionSliders();
    
    MainWindow *mainWindow();
    void setMainWindow(MainWindow *mainWindow);
    
signals:
    void timeChanged(const QDateTime& time);
    void lonChanged(qreal lon);
    void latChanged(qreal lat);
    void heightChanged(qreal height);
    
private slots:
    void updateLimits();
    void updateShownSliders(MainWindow::ViewMode viewMode);
    
    void setTime(const QDateTime& time);
    void setLon(qreal lon);
    void setLat(qreal lat);
    void setHeight(qreal height);
    
    void handleChangedTimeValue(int timeValue);
    void handleChangedLonValue(int lonValue);
    void handleChangedLatValue(int latValue);
    void handleChangedHeightValue(int heightValue);
    
    void setCoverageId(const QString& coverageId);
    
private:
    void connectSliders();
    void disconnectSliders();
    
    void showTimeSlider(bool show);
    void showLonSlider(bool show);
    void showLatSlider(bool show);
    void showHeightSlider(bool show);
    
    QList<CoordinateAxis> coordinateAxes() const;
    bool axisAvailable(Dimension dimension) const;
    
    MainWindow *m_mainWindow;
    
    Ui::DimensionSliders ui;
    QDateTime m_minTime;
    QDateTime m_maxTime;
    int m_valueCountTime;
    float m_minLon;
    float m_maxLon;
    int m_valueCountLon;
    float m_minLat;
    float m_maxLat;
    int m_valueCountLat;
    float m_minHeight;
    float m_maxHeight;
    int m_valueCountHeight;
    QString m_coverageId;
};

#endif
