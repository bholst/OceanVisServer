//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// STD
#include <cmath>

// Qt
#include <QtCore/QDebug>
#include <QtCore/QXmlStreamWriter>

// Project
#include "XmlStreamReader.h"

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

void ColorMap::setName(const QString& name)
{
    m_name = name;
}

QString ColorMap::name() const
{
    return m_name;
}

QColor ColorMap::color(qreal value)
{
    if(m_colors.size() == 0) {
        return Qt::black;
    }
    else if(m_colors.size() == 1) {
        return m_colors[0];
    }
    
    // Find out between which two colors our color will be
    qreal colorIndex = value * ((qreal) m_colors.size() - 1.0);
    
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
        if(h1 == -1.0 && h2 == -1.0) {
            h1 = 0.0;
            h2 = 0.0;
        }
        else if(h1 == -1.0) {
            h1 = h2;
        }
        else if(h2 == -1.0) {
            h2 = h1;
        }
        
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
        if(h1 == -1.0 && h2 == -1.0) {
            h1 = 0.0;
            h2 = 0.0;
        }
        else if(h1 == -1.0) {
            h1 = h2;
        }
        else if(h2 == -1.0) {
            h2 = h1;
        }
        
        QColor resultingColor;
        resultingColor.setHslF(nRemainingFraction * h1 + remainingFraction * h2,
                               nRemainingFraction * s1 + remainingFraction * s2,
                               nRemainingFraction * l1 + remainingFraction * l2,
                               nRemainingFraction * a1 + remainingFraction * a2);
        return resultingColor;
    }
    else {
        return Qt::black;
    }
}

ColorMap ColorMap::readColorMap(XmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement()
             && reader->name() == "colorMap");
    
    ColorMap colorMap;
    colorMap.setName(reader->attributes().value("name").toString());
    
    while(!reader->atEnd()) {
        reader->readNext();
        
        if(reader->isEndElement()) {
            break;
        }

        if(reader->isStartElement()) {
            if(reader->name() == "color") {
                colorMap.addColor(readColor(reader));
            }
            else if(reader->name() == "interpolationSpec") {
                QString intSpec = reader->readCharacters();
                if(intSpec == "rgb") {
                    colorMap.setInterpolationSpec(QColor::Rgb);
                }
                else if(intSpec == "hsv") {
                    colorMap.setInterpolationSpec(QColor::Hsv);
                }
                else if(intSpec == "cmyk") {
                    colorMap.setInterpolationSpec(QColor::Cmyk);
                }
                else if(intSpec == "hsl") {
                    colorMap.setInterpolationSpec(QColor::Hsl);
                }
            }
            else {
                reader->readUnknownElement();
            }
        }
    }
    
    return colorMap;
}

void ColorMap::writeColorMap(QXmlStreamWriter *writer)
{
    writer->writeStartElement("colorMap");
    writer->writeAttribute("name", name());
    
    writer->writeStartElement("interpolationSpec");
    switch(interpolationSpec()) {
        case QColor::Rgb:
            writer->writeCharacters("rgb");
            break;
        case QColor::Hsv:
            writer->writeCharacters("hsv");
            break;
        case QColor::Cmyk:
            writer->writeCharacters("cmyk");
            break;
        case QColor::Hsl:
            writer->writeCharacters("hsl");
            break;
    }
    writer->writeEndElement();
    
    foreach(QColor color, colors()) {
        writeColor(color, writer);
    }
    
    writer->writeEndElement();
}

QColor ColorMap::readColor(XmlStreamReader *reader)
{
    Q_ASSERT(reader->isStartElement()
             && reader->name() == "color");

    QColor::Spec spec = QColor::Rgb;
    QXmlStreamAttributes colorAtt = reader->attributes();
    if(colorAtt.hasAttribute("spec")) {
        QString specString = colorAtt.value("spec").toString();
        if(specString == "rgb") {
            spec = QColor::Rgb;
        }
        else if(specString == "hsv") {
            spec = QColor::Hsv;
        }
        else if(specString == "cmyk") {
            spec = QColor::Cmyk;
        }
        else if(specString == "hsl") {
            spec = QColor::Hsl;
        }
    }
    
    QString colorStr = reader->readCharacters();
    QStringList colorStrings = colorStr.split(',', QString::SkipEmptyParts);
    QList<int> colorValues;
    foreach(QString colorString, colorStrings) {
        colorValues.append(colorString.toInt());
    }
    QColor color;
    
    if(colorValues.size() == 3) {
        switch(spec) {
            case QColor::Rgb:
                color.setRgb(colorValues[0], colorValues[1], colorValues[2]);
                break;
            case QColor::Hsv:
                color.setHsv(colorValues[0], colorValues[1], colorValues[2]);
                break;
            case QColor::Hsl:
                color.setHsl(colorValues[0], colorValues[1], colorValues[2]);
                break;
            default:
                qDebug() << "Color: Spec and number of values don't agree.";
                break;
        }
    }
    else if(colorValues.size() == 4) {
        switch(spec) {
            case QColor::Rgb:
                color.setRgb(colorValues[0], colorValues[1], colorValues[2], colorValues[3]);
                break;
            case QColor::Hsv:
                color.setHsv(colorValues[0], colorValues[1], colorValues[2], colorValues[3]);
                break;
            case QColor::Hsl:
                color.setHsl(colorValues[0], colorValues[1], colorValues[2], colorValues[3]);
                break;
            case QColor::Cmyk:
                color.setCmyk(colorValues[0], colorValues[1], colorValues[2], colorValues[3]);
                break;
            default:
                qDebug() << "Color: Spec and number of values don't agree.";
                break;
        }
    }
    else if(colorValues.size() == 5 && spec == QColor::Cmyk) {
        color.setCmyk(colorValues[0], colorValues[1], colorValues[2], colorValues[3], colorValues[4]);
    }
    else {
        color.setCmyk(colorValues[0], colorValues[1], colorValues[2], colorValues[3]);
    }
    
    return color;
}

void ColorMap::writeColor(QColor color, QXmlStreamWriter *writer)
{
    writer->writeStartElement("color");
    
    switch(interpolationSpec()) {
        case QColor::Rgb:
            writer->writeAttribute("spec", "rgb");
            int r, g, b, a;
            color.getRgb(&r, &g, &b, &a);
            writer->writeCharacters(QString("%1, %2, %3, %4").arg(r).arg(g).arg(b).arg(a));
            break;
        case QColor::Hsv:
            writer->writeAttribute("spec", "hsv");
            int h, s, v;
            color.getHsv(&h, &s, &v, &a);
            writer->writeCharacters(QString("%1, %2, %3, %4").arg(h).arg(s).arg(v).arg(a));
            break;
        case QColor::Cmyk:
            writer->writeAttribute("spec", "cmyk");
            int c, m, y, k;
            color.getCmyk(&c, &m, &y, &k, &a);
            writer->writeCharacters(QString("%1, %2, %3, %4, %5").arg(c).arg(m).arg(y).arg(k).arg(a));
            break;
        case QColor::Hsl:
            writer->writeAttribute("spec", "hsl");
            int l;
            color.getHsl(&h, &s, &l, &a);
            writer->writeCharacters(QString("%1, %2, %3, %4").arg(h).arg(s).arg(l).arg(a));
            break;
    }
    
    writer->writeEndElement();
}
