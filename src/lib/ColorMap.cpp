//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <cmath>

// Self
#include "ColorMap.h"

ColorMap::ColorMap()
    : m_interpolationSpec(QColor::Rgb)
{
}

ColorMap::~ColorMap()
{
}

void ColorMap::setInterpolationSpec(QColor::Spec spec)
{
    m_interpolationSpec = spec;
}

QColor::Spec ColorMap::interpolationSpec() const
{
    return m_interpolationSpec;
}

void ColorMap::setColors(const QList<QColor>& colors)
{
    m_colors = colors;
}

QList<QColor> ColorMap::colors() const
{
    return m_colors;
}

void ColorMap::addColor(const QColor& color)
{
    m_colors.append(color);
}

QColor ColorMap::color(qreal value)
{
    if(m_colors.size() == 0) {
        return Qt::black;
    }
    
    // Find out between which two colors our color will be
    qreal widthPerColor = 1.0 / ((qreal) m_colors.size() - 1.0);
    qreal colorIndex = value / widthPerColor;
    
    int lowerIndex = std::max(0, std::min((int) std::floor(colorIndex), m_colors.size() - 1));
    int upperIndex = std::max(0, std::min((int) std::ceil(colorIndex), m_colors.size() - 1));
    
    // Then calculating a remaining fraction for the interpolation.
    qreal remainingFraction = colorIndex - std::floor(colorIndex);
    qreal nRemainingFraction = 1.0 - remainingFraction;
    
    if(QColor::Rgb == m_interpolationSpec) {
        qreal firstRed, secondRed, firstGreen, secondGreen, firstBlue, secondBlue, firstAlpha, secondAlpha;
        m_colors[lowerIndex].getRgbF(&firstRed, &firstGreen, &firstBlue, &firstAlpha);
        m_colors[upperIndex].getRgbF(&secondRed, &secondGreen, &secondBlue, &secondAlpha);
        
        QColor resultingColor;
        resultingColor.setRgbF(nRemainingFraction * firstRed + remainingFraction * secondRed,
                               nRemainingFraction * firstGreen + remainingFraction * secondGreen,
                               nRemainingFraction * firstBlue + remainingFraction * secondBlue,
                               nRemainingFraction * firstAlpha + remainingFraction * secondAlpha);
        return resultingColor;
    }
    else if(QColor::Hsv == m_interpolationSpec) {
        qreal h1, h2, s1, s2, v1, v2, a1, a2;
        m_colors[lowerIndex].getHsvF(&h1, &s1, &v1, &a1);
        m_colors[upperIndex].getHsvF(&h2, &s2, &v2, &a2);
        
        QColor resultingColor;
        resultingColor.setHsvF(nRemainingFraction * h1 + remainingFraction * h2,
                               nRemainingFraction * s1 + remainingFraction * s2,
                               nRemainingFraction * v1 + remainingFraction * v2,
                               nRemainingFraction * a1 + remainingFraction * a2);
        return resultingColor;
    }
    else if(QColor::Cmyk == m_interpolationSpec) {
        qreal c1, c2, m1, m2, y1, y2, k1, k2, a1, a2;
        m_colors[lowerIndex].getCmykF(&c1, &m1, &y1, &k1, &a1);
        m_colors[upperIndex].getCmykF(&c2, &m2, &y2, &k2, &a2);
        
        QColor resultingColor;
        resultingColor.setCmykF(nRemainingFraction * c1 + remainingFraction * c2,
                                nRemainingFraction * m1 + remainingFraction * m2,
                                nRemainingFraction * y1 + remainingFraction * y2,
                                nRemainingFraction * k1 + remainingFraction * k2,
                                nRemainingFraction * a1 + remainingFraction * a2);
        return resultingColor;
    }
    else if(QColor::Hsl == m_interpolationSpec) {
        qreal h1, h2, s1, s2, l1, l2, a1, a2;
        m_colors[lowerIndex].getHslF(&h1, &s1, &l1, &a1);
        m_colors[upperIndex].getHslF(&h2, &s2, &l2, &a2);
        
        QColor resultingColor;
        resultingColor.setHsvF(nRemainingFraction * h1 + remainingFraction * h2,
                               nRemainingFraction * s1 + remainingFraction * s2,
                               nRemainingFraction * l1 + remainingFraction * l2,
                               nRemainingFraction * a1 + remainingFraction * a2);
        return resultingColor;
    }
    else {
        return Qt::black;
    }
}
