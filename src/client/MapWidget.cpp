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

const int MAPWIDGET_SCALE_MARGIN = 60;
const int MAPWIDGET_NORMAL_MARGIN = 20;
const QColor MAPWIDGET_BACKGROUND_COLOR = QColor(0, 0, 0, 255);
const QColor MAPWIDGET_LINE_COLOR = QColor(255, 255, 255, 255);

MapWidget::MapWidget(QWidget * parent, Qt::WindowFlags f)
    : QWidget(parent, f),
      m_manager(new QNetworkAccessManager(this)),
      m_downloadId(0),
      m_shownId(0),
      m_fitWindow(false)
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
    
    update();
}

void MapWidget::setYAxis(const QString& string, const QString& min, const QString& max)
{
    m_yAxisString = string;
    m_yAxisMin = min;
    m_yAxisMax = max;
    
    update();
}

bool MapWidget::fitWindow() const
{
    return m_fitWindow;
}

void MapWidget::setFitWindow(bool fitWindow)
{
    m_fitWindow = fitWindow;
    update();
}

void MapWidget::paintEvent(QPaintEvent *event)
{
    QRect imageRect(MAPWIDGET_SCALE_MARGIN,
                    MAPWIDGET_NORMAL_MARGIN,
                    width() - MAPWIDGET_NORMAL_MARGIN - MAPWIDGET_SCALE_MARGIN,
                    height() - MAPWIDGET_NORMAL_MARGIN - MAPWIDGET_SCALE_MARGIN);
    
    qreal scaleX = (qreal) m_image.width() / (qreal) imageRect.width();
    qreal scaleY = (qreal) m_image.height() / (qreal) imageRect.height();
    
    if(!m_fitWindow) {
        if(scaleX > scaleY) {
            scaleY = scaleX;
            qreal addBorderY = (imageRect.height() - m_image.height() / scaleX) / 2.0;
            imageRect.setTop(imageRect.top() + addBorderY);
            imageRect.setBottom(imageRect.bottom() - addBorderY);
        }
        else {
            scaleX = scaleY;
            qreal addBorderX = (imageRect.width() - m_image.width() / scaleY) / 2.0;
            imageRect.setLeft(imageRect.left() + addBorderX);
            imageRect.setRight(imageRect.right() - addBorderX);
        }
    }
    
    QRect dirtyRect = event->rect();
    QPainter painter(this);
    painter.fillRect(dirtyRect, MAPWIDGET_BACKGROUND_COLOR);
    painter.setPen(MAPWIDGET_LINE_COLOR);
    painter.drawRect(QRect(imageRect.x() - 1, imageRect.y() - 1, imageRect.width() + 1, imageRect.height() + 1));
    
    QRectF dirtyImageF = dirtyRect & imageRect;
    if(!dirtyImageF.isEmpty()) {
        if(m_image.width() == 0 || m_image.height() == 0) {
            return;
        }
    
        QRectF sourceRect((dirtyImageF.x() - imageRect.x()) * scaleX, (dirtyImageF.y() - imageRect.y()) * scaleY,
                          dirtyImageF.width() * scaleX, dirtyImageF.height() * scaleY);
        painter.drawImage(dirtyImageF, m_image, sourceRect);
    }
    
    // X Axis
    painter.drawLine(imageRect.bottomLeft() + QPoint(-1, 5), imageRect.bottomLeft() + QPoint(-1, 12));
    painter.drawLine(imageRect.bottomRight() + QPoint(1, 5), imageRect.bottomRight() + QPoint(1, 12));
    
    QFont previousFont = painter.font();
    
    while(painter.fontMetrics().size(Qt::AlignLeft | Qt::AlignVCenter, m_xAxisMin).width() > imageRect.width() / 2
           || painter.fontMetrics().size(Qt::AlignRight | Qt::AlignVCenter, m_yAxisMin).width() > imageRect.width() / 2)
    {
        QFont font = painter.font();
        font.setPointSize(font.pointSize() - 1);
        painter.setFont(font);
        if(font.pointSize() <= 1) {
            break;
        }
    }
    QRect minXTextRect(imageRect.bottomLeft() + QPoint(-1, 15), QSize(imageRect.width() / 2, 20));
    painter.drawText(minXTextRect, Qt::AlignLeft | Qt::AlignVCenter, m_xAxisMin);
    QRect maxXTextRect(imageRect.bottomRight() + QPoint(1 - imageRect.width() / 2, 15), QSize(imageRect.width() / 2, 20));
    painter.drawText(maxXTextRect, Qt::AlignRight | Qt::AlignVCenter, m_xAxisMax);
    
    painter.setFont(previousFont);
    QRect stringXTextRect(imageRect.bottomLeft() + QPoint(-1, 40), QSize(imageRect.width(), 20));
    painter.drawText(stringXTextRect, Qt::AlignCenter | Qt::AlignVCenter, m_xAxisString);
    
    // Y Axis
    painter.setTransform(QTransform().translate(imageRect.bottomLeft().x() - 60, imageRect.bottomLeft().y() + 1)
                                     .rotate(270));
    QRect stringYTextRect(QPoint(0,0), QSize(imageRect.height() + 1, 20));
    painter.drawText(stringYTextRect, Qt::AlignCenter | Qt::AlignVCenter, m_yAxisString);
    
    QRect minYTextRect(QPoint(0, 20), QSize(imageRect.height() / 2, 20));
    painter.drawText(minYTextRect, Qt::AlignLeft | Qt::AlignVCenter, m_yAxisMin);
    QRect maxYTextRect(QPoint(imageRect.height() / 2 + 1, 20), QSize(imageRect.height() / 2, 20));
    painter.drawText(maxYTextRect, Qt::AlignRight | Qt::AlignVCenter, m_yAxisMax);
    
    painter.drawLine(QPoint(0, 45), QPoint(0, 55));
    painter.drawLine(QPoint(imageRect.height() + 1, 45), QPoint(imageRect.height() + 1, 55));
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
