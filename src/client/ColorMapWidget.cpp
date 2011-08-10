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

const int LABEL_WIDTH = 100;
const int TEXT_WIDTH = LABEL_WIDTH - 20;
const int COLOR_RECT_MARGIN = 15;

ColorMapWidget::ColorMapWidget(QWidget * parent, Qt::WindowFlags f)
    : QWidget(parent, f),
      m_colorMap(),
      m_minValue(0.0),
      m_maxValue(0.0)
{
    setMinimumWidth(LABEL_WIDTH * 2 + COLOR_RECT_MARGIN * 2);
    setMinimumHeight(LABEL_WIDTH * 2 + COLOR_RECT_MARGIN * 2);
}

void ColorMapWidget::setColorMap(const ColorMap& colorMap)
{
    m_colorMap = colorMap;
    update();
}

ColorMap ColorMapWidget::colorMap() const
{
    return m_colorMap;
}

void ColorMapWidget::setMinValue(double minValue)
{
    m_minValue = minValue;
}

void ColorMapWidget::setMaxValue(double maxValue)
{
    m_maxValue = maxValue;
}

void ColorMapWidget::paintEvent(QPaintEvent *event)
{
    QRect dirtyRect = event->rect();
    QPainter painter(this);
    
    painter.fillRect(dirtyRect, QColor(0, 0, 0, 255));
    
    // Calculate Text Sizes
    QFontMetrics metrics = painter.fontMetrics();
    QString maxString = QString::number(m_maxValue, 'g', 3);
    QString mediumString = QString::number(m_minValue / 2 + m_maxValue / 2, 'g', 3);
    QString minString = QString::number(m_minValue, 'g', 3);
    
    int maxTextWidth = 0;
    {
        int maxWidth = metrics.size(Qt::AlignLeft | Qt::AlignVCenter, maxString).width();
        int mediumWidth = metrics.size(Qt::AlignLeft | Qt::AlignVCenter, mediumString).width();
        int minWidth = metrics.size(Qt::AlignLeft | Qt::AlignVCenter, minString).width();
            
        int maxTextWidth = (maxWidth > mediumWidth) ? maxWidth : mediumWidth;
        if(minWidth > maxTextWidth) {
            maxTextWidth = minWidth;
        }
    }
    
    int rightColorRectMargin = COLOR_RECT_MARGIN * 2 + 20 + maxTextWidth;
    
    QRect colorRect(QPoint(COLOR_RECT_MARGIN, COLOR_RECT_MARGIN),
                    QSize(width() - rightColorRectMargin - COLOR_RECT_MARGIN, height() - 2 * COLOR_RECT_MARGIN));
    
    painter.save();
    
    painter.translate(colorRect.topLeft());
    QRect dirtyColorRect = dirtyRect.intersected(colorRect);
    dirtyColorRect.translate(-colorRect.topLeft().x(), -colorRect.topLeft().y());
    
    for(int j = dirtyColorRect.top(); j <= dirtyColorRect.bottom(); ++j) {
        painter.setPen(m_colorMap.color(1.0 - (qreal) j / (qreal) colorRect.height()));
        painter.drawLine(dirtyColorRect.left(), j, dirtyColorRect.right(), j);
    }
    
    painter.restore();
    
    painter.setPen(QColor(255, 255, 255, 255));
    painter.translate(colorRect.topRight() + QPoint(1, 0));
    painter.drawLine(QPoint(5, 0), QPoint(15, 0));
    painter.drawLine(QPoint(5, colorRect.height() / 2 - 1), QPoint(15, colorRect.height() / 2 - 1));
    painter.drawLine(QPoint(5, colorRect.height() - 1), QPoint(15, colorRect.height() - 1));
    
    painter.drawText(QRect(QPoint(20, -10), QSize(LABEL_WIDTH, 20)),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     maxString);
    painter.drawText(QRect(QPoint(20, colorRect.height() / 2 - 1 - 10), QSize(LABEL_WIDTH, 20)),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     mediumString);
    painter.drawText(QRect(QPoint(20, colorRect.height() - 1 - 10), QSize(LABEL_WIDTH, 20)),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     minString);
}

#include "ColorMapWidget.moc"
