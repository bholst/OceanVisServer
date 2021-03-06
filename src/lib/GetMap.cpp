//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Project
#include "DimensionTrim.h"
#include "DimensionSlice.h"
#include "ParseTime.h"

// Self
#include "GetMap.h"

GetMap::GetMap()
    : RequestBase(RequestBase::WMS),
      m_width(0),
      m_height(0),
      m_format("image/png"),
      m_transparent(false),
      m_backgroundColor(0, 0, 0),
      m_time(QDateTime::currentDateTime()),
      m_elevation(0)
{
}

GetMap::~GetMap()
{
    qDeleteAll(m_dimensionSubsets);
}

QString GetMap::request() const
{
    return QString("GetMap");
}

QList<DimensionSubset*> GetMap::dimensionSubsets() const
{
    if(m_dimensionSubsetsChanged) {
        qDeleteAll(m_dimensionSubsets);
        m_dimensionSubsets.clear();
        
        DimensionSlice *time = new DimensionSlice(Time);
        time->setSlicePoint(m_time);
        m_dimensionSubsets.append(time);
        
        DimensionTrim *lon = new DimensionTrim(Lon);
        lon->setTrimLow(m_boundingBox.lonMin());
        lon->setTrimHigh(m_boundingBox.lonMax());
        m_dimensionSubsets.append(lon);
        
        DimensionTrim *lat = new DimensionTrim(Lat);
        lat->setTrimLow(m_boundingBox.latMin());
        lat->setTrimHigh(m_boundingBox.latMax());
        m_dimensionSubsets.append(lat);
        
        DimensionSlice *height = new DimensionSlice(Height);
        height->setSlicePoint(m_elevation);
        m_dimensionSubsets.append(height);
        
        m_dimensionSubsetsChanged = false;
    }
    
    return m_dimensionSubsets;
}

QStringList GetMap::layers() const
{
    return m_layers;
}

void GetMap::setLayers(const QStringList& layers)
{
    m_layers = layers;
}

void GetMap::addLayer(const QString& layer)
{
    m_layers.append(layer);
}

QStringList GetMap::styles() const
{
    return m_styles;
}

void GetMap::setStyles(const QStringList& styles)
{
    m_styles = styles;
}

void GetMap::addStyle(const QString& style)
{
    m_styles.append(style);
}

BoundingBox GetMap::boundingBox() const
{
    return m_boundingBox;
}

void GetMap::setBoundingBox(const BoundingBox& boundingBox)
{
    m_dimensionSubsetsChanged = true;
    m_boundingBox = boundingBox;
}

int GetMap::width() const
{
    return m_width;
}

void GetMap::setWidth(int width)
{
    m_width = width;
}

int GetMap::height() const
{
    return m_height;
}

void GetMap::setHeight(int height)
{
    m_height = height;
}

void GetMap::setFormat(const QString& format)
{
    m_format = format;
}

QString GetMap::format() const
{
    return m_format;
}

bool GetMap::transparent() const
{
    return m_transparent;
}

void GetMap::setTransparent(bool transparent)
{
    m_transparent = transparent;
}

QColor GetMap::backgroundColor() const
{
    return m_backgroundColor;
}

void GetMap::setBackgroundColor(const QColor& backgroundColor)
{
    m_backgroundColor = backgroundColor;
}

QDateTime GetMap::time() const
{
    return m_time;
}

void GetMap::setTime(const QDateTime& time)
{
    m_dimensionSubsetsChanged = true;
    m_time = time;
}

int GetMap::elevation() const
{
    return m_elevation;
}

void GetMap::setElevation(int elevation)
{
    m_dimensionSubsetsChanged = true;
    m_elevation = elevation;
}

GetMap *GetMap::fromRequestString(QString request)
{
    QString version;
    QStringList styles;
    QStringList layers;
    BoundingBox boundingBox;
    int width = 0;
    int height = 0;
    bool transparent = false;
    QColor backgroundColor;
    QDateTime time = QDateTime::currentDateTime();
    int elevation = 0;
    
    QStringList args = request.split('&');
    
    foreach(QString arg, args) {
        QStringList argParts = arg.split('=');
        if(argParts.size() != 2) {
            return 0;
        }
        QString field = argParts[0].toLower();
        QString value = argParts[1];
        
        if(field == "version") {
            version = value;
        }
        else if(field == "layers") {
            layers = value.split(',');
        }
        else if(field == "styles") {
            styles = value.split(',');
        }
        else if(field == "crs" && value != "EPSG:4326") {
            return 0;
        }
        else if(field == "bbox") {
            QStringList borders = value.split(',');
            if(borders.size() != 4) 
                return 0;
            
            
            boundingBox.setLonMin(borders.at(0).toDouble());
            boundingBox.setLatMin(borders.at(1).toDouble());
            boundingBox.setLonMax(borders.at(2).toDouble());
            boundingBox.setLatMax(borders.at(3).toDouble());
        }
        else if(field == "width") {
            width = value.toInt();
        }
        else if(field == "height") {
            height = value.toInt();
        }
        else if(field == "transparent") {
            if(value.toLower() == "true") {
                transparent = true;
            }
            else {
                transparent = false;
            }
        }
        else if(field == "bgcolor") {
            bool ok;
            uint colorVal = value.toUInt(&ok, 0);
            backgroundColor.setRgb(colorVal>>16 & 0xFF, colorVal>>8 & 0xFF,colorVal & 0xFF);
        }
        else if(field == "exceptions" && value.toLower() != "xml") {
            return 0;
        }
        else if(field == "time") {
            time = parseTime(value);
        }
        else if(field == "elevation") {
            elevation = value.toInt();
        }
    }
    
    GetMap *getMap = new GetMap();
    getMap->setVersion(version);
    getMap->setLayers(layers);
    getMap->setStyles(styles);
    getMap->setBoundingBox(boundingBox);
    getMap->setWidth(width);
    getMap->setHeight(height);
    getMap->setTransparent(transparent);
    getMap->setBackgroundColor(backgroundColor);
    getMap->setTime(time);
    getMap->setElevation(elevation);
    return getMap;
}

QString GetMap::toString() const
{
    QString result("Getmap (%1):\n"
                   "VERSION=%2\n"
                   "LAYERS=%3\n"
                   "STYLES=%4\n"
                   "BBOX=%5,%6,%7,%8\n"
                   "WIDTH=%9\n"
                   "HEIGHT=%10\n"
                   "TRANSPARENT=%11\n"
                   "BGCOLOR=0x%12%13%14\n"
                   "TIME=%15\n"
                   "ELEVATION=%16\n");
    
    result = result.arg((unsigned long) this, 0, 16);
    result = result.arg(version());
    result = result.arg(layers().join(QChar(',')));
    result = result.arg(styles().join(QChar(',')));
    result = result.arg(m_boundingBox.lonMin()).arg(m_boundingBox.latMin()).arg(m_boundingBox.lonMax()).arg(m_boundingBox.latMax());
    result = result.arg(width());
    result = result.arg(height());
    if(transparent()) {
        result = result.arg("TRUE");
    }
    else {
        result = result.arg("FALSE");
    }
    result = result.arg(m_backgroundColor.red(), 2, 16, QChar('0'))
                   .arg(m_backgroundColor.green(), 2, 16, QChar('0'))
                   .arg(m_backgroundColor.blue(), 2, 16, QChar('0'));
    result = result.arg(m_time.toString(Qt::ISODate));
    result = result.arg(m_elevation);
    return result;
}
