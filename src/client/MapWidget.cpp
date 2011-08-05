//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <cstdio>

// Qt
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QRectF>
#include <QtGui/QImage>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

// Self
#include "MapWidget.h"

const int MAPWIDGET_SCALE_MARGIN = 40;
const int MAPWIDGET_NORMAL_MARGIN = 20;
const QColor MAPWIDGET_BACKGROUND_COLOR = QColor(0, 0, 0, 255);
const QColor MAPWIDGET_LINE_COLOR = QColor(255, 255, 255, 255);

MapWidget::MapWidget(QWidget * parent, Qt::WindowFlags f)
    : QWidget(parent, f),
      m_manager(new QNetworkAccessManager(this)),
      m_downloadId(0),
      m_shownId(0)
{
    connect(m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleReply(QNetworkReply*)));

}

MapWidget::~MapWidget()
{
}

QString MapWidget::url() const
{
    return m_url;
}

void MapWidget::setUrl(const QString& url)
{
    m_url = url;
    
    m_ids.insert(url, ++m_downloadId);
    m_manager->get(QNetworkRequest(QUrl(url)));
}

void MapWidget::setXAxis(const QString& string, const QString& min, const QString& max)
{
    m_xAxisString = string;
    m_xAxisMin = min;
    m_xAxisMax = max;
}

void MapWidget::setYAxis(const QString& string, const QString& min, const QString& max)
{
    m_yAxisString = string;
    m_yAxisMin = min;
    m_yAxisMax = max;
}

void MapWidget::paintEvent(QPaintEvent *event)
{
    QRect imageRect(MAPWIDGET_SCALE_MARGIN,
                    MAPWIDGET_NORMAL_MARGIN,
                    width() - MAPWIDGET_NORMAL_MARGIN - MAPWIDGET_SCALE_MARGIN,
                    height() - MAPWIDGET_NORMAL_MARGIN - MAPWIDGET_SCALE_MARGIN);
    
    qreal scaleX = (qreal) m_image.width() / (qreal) imageRect.width();
    qreal scaleY = (qreal) m_image.height() / (qreal) imageRect.height();
    
    qreal scale;
    if(scaleX > scaleY) {
        scale = scaleX;
        qreal addBorderY = (imageRect.height() - m_image.height() / scale) / 2.0;
        imageRect.setTop(imageRect.top() + addBorderY);
        imageRect.setBottom(imageRect.bottom() - addBorderY);
    }
    else {
        scale = scaleY;
        qreal addBorderX = (imageRect.width() - m_image.width() / scale) / 2.0;
        imageRect.setLeft(imageRect.left() + addBorderX);
        imageRect.setRight(imageRect.right() - addBorderX);
    }
    
    QRect dirtyRect = event->rect();
    QPainter painter(this);
    painter.fillRect(dirtyRect, MAPWIDGET_BACKGROUND_COLOR);
    painter.setPen(MAPWIDGET_LINE_COLOR);
    painter.drawRect(QRect(imageRect.x() - 1, imageRect.y() - 1, imageRect.width() + 2, imageRect.height() + 2));
    
    QRectF dirtyImageF = dirtyRect & imageRect;
    if(!dirtyImageF.isEmpty()) {
        if(m_image.width() == 0 || m_image.height() == 0) {
            return;
        }
    
        QRectF sourceRect((dirtyImageF.x() - imageRect.x()) * scale, (dirtyImageF.y() - imageRect.y()) * scale,
                          dirtyImageF.width() * scale, dirtyImageF.height() * scale);
        painter.drawImage(dirtyImageF, m_image, sourceRect);
    }
}

void MapWidget::handleReply(QNetworkReply *reply)
{
    QString url = reply->url().toString();
    if(reply->error()) {
        fprintf(stderr, "Download of %s failed: %s\n",
                url.toStdString().c_str(),
                qPrintable(reply->errorString()));
    }
    else {
        unsigned long long currentId = m_ids.value(url);
        if(currentId > m_shownId) {
            if(m_image.load(reply, "png")) {
                update();
            }
            m_shownId = currentId;
        }
    }
    
    m_ids.remove(url);
    reply->deleteLater();
}

#include "MapWidget.moc"
