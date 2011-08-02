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

void MapWidget::paintEvent(QPaintEvent *event)
{
    QRect dirtyRect = event->rect();
    QRectF dirtyRectF = dirtyRect;
    QPainter painter(this);
    painter.fillRect(dirtyRect, QColor(0, 0, 0, 255));
    
    if(m_image.width() == 0 || m_image.height() == 0) {
        return;
    }
    qreal scaleX = (qreal) m_image.width() / (qreal) width();
    qreal scaleY = (qreal) m_image.height() / (qreal) height();
    qreal scale = (scaleX > scaleY) ? scaleX : scaleY;
    
    QRectF sourceRect(dirtyRectF.x() * scale, dirtyRectF.y() * scale,
                      dirtyRectF.width() * scale, dirtyRectF.height() * scale);
    painter.drawImage(dirtyRectF, m_image, sourceRect);
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
