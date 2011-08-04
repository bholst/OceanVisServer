//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Qt
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>

// Project
#include "ColorMap.h"

// Self
#include "ColorMapWidget.h"

ColorMapWidget::ColorMapWidget(QWidget * parent, Qt::WindowFlags f)
    : QWidget(parent, f),
      m_colorMap()
{
}

void ColorMapWidget::setColorMap(const ColorMap& colorMap)
{
    m_colorMap = colorMap;
}

ColorMap ColorMapWidget::colorMap() const
{
    return m_colorMap;
}

void ColorMapWidget::paintEvent(QPaintEvent *event)
{
    QRect dirtyRect = event->rect();
    QPainter painter(this);
    painter.fillRect(dirtyRect, QColor(0, 0, 0, 255));
    
    for(int j = dirtyRect.top(); j <= dirtyRect.bottom(); ++j) {
        painter.setPen(m_colorMap.color(1.0 - (qreal) j / (qreal) height()));
        painter.drawLine(dirtyRect.left(), j, dirtyRect.right(), j);
    }
}

#include "ColorMapWidget.moc"
