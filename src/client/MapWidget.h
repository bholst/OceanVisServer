//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef MAPWIDGET_H
#define MAPWIDGET_H

// Qt
#include <QtCore/QMap>
#include <QtGui/QWidget>

class QNetworkAccessManager;
class QNetworkReply;

class MapWidget : public QWidget {
    Q_OBJECT

public:
    MapWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~MapWidget();
    
    QString url() const;
    
    bool fitWindow() const;
    
public slots:
    void setUrl(const QString& url);
    void setXAxis(const QString& string, const QString& min, const QString& max);
    void setYAxis(const QString& string, const QString& min, const QString& max);
    
    void setFitWindow(bool fitWindow);
    
protected:
    void paintEvent(QPaintEvent *event);
    
private slots:
    void handleReply(QNetworkReply *reply);
    
private:
    QString m_url;
    QImage m_image;
    QNetworkAccessManager *m_manager;
    
    unsigned long long m_downloadId;
    unsigned long long m_shownId;
    QMap<QString, unsigned long long> m_ids;
    
    QString m_xAxisString;
    QString m_xAxisMin;
    QString m_xAxisMax;
    
    QString m_yAxisString;
    QString m_yAxisMin;
    QString m_yAxisMax;
    
    bool m_fitWindow;
};

#endif