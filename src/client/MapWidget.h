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
    
public slots:
    void setUrl(const QString& url);
    
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
};

#endif