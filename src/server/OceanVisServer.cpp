//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Standard
#include <iostream>

// Qt
#include <QtAlgorithms>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QBuffer>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <QtCore/QRegExp>
#include <QtCore/QBuffer>
#include <QtCore/QDataStream>
#include <QtCore/QLocale>
#include <QtGui/QImageWriter>
#include <QtGui/QImage>
#include <QtGui/QPainter>

// Project
#include "Dimension.h"
#include "DataLayer.h"
#include "RequestBase.h"
#include "GetCoverage.h"
#include "GetMap.h"
#include "DescribeCoverages.h"
#include "DimensionTrim.h"
#include "DimensionSlice.h"
#include "GridCoverage.h"
#include "RequestParser.h"
#include "ResponseWriter.h"
#include "CoordinateAxis.h"

// Self
#include "OceanVisServer.h"

OceanVisServer::OceanVisServer(QObject *parent)
    : disabled(false),
      m_lastModified(QDateTime::currentDateTime().toUTC())
{
}

OceanVisServer::~OceanVisServer()
{
    qDeleteAll(m_layers);
}

void OceanVisServer::incomingConnection(int socket)
{
    if(disabled)
        return;

    QTcpSocket *s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socket);

    qDebug() << "New Connection";
}

void OceanVisServer::pause()
{
    disabled = true;
}

void OceanVisServer::resume()
{
    disabled = false;
}

void OceanVisServer::setLayers(const QHash<QString,DataLayer *>& layers)
{
    m_layers = layers;
    m_lastModified = QDateTime::currentDateTime().toUTC();
}

QHash<QString,DataLayer *> OceanVisServer::layers() const
{
    return m_layers;
}

void OceanVisServer::readClient()
{
    if(disabled)
        return;

    QTcpSocket *socket = (QTcpSocket *) sender();

    if(socket->canReadLine()) {        
        RequestBase *request = 0;

        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if(tokens[0] == "GET") {
            QString url = tokens[1];
            QStringList urlSplit = url.split('?');
            if(urlSplit.size() < 2) {
                qDebug() << "Invalid request";
                return;
            }
            
            QRegExp serviceType("SERVICE=(wcs|wms|ovp)");
            serviceType.setCaseSensitivity(Qt::CaseInsensitive);
            serviceType.indexIn(urlSplit.at(1));
            QRegExp requestType("REQUEST=(GetMap|GetCoverage|DescribeCoverages)");
            requestType.setCaseSensitivity(Qt::CaseInsensitive);
            requestType.indexIn(urlSplit.at(1));

            if(serviceType.pos() <= -1) {
                return;
            }
            if(QString::compare(serviceType.cap(1), "wcs", Qt::CaseInsensitive) == 0
               && QString::compare(requestType.cap(1), "GetCoverage", Qt::CaseInsensitive) == 0)
            {
                request = GetCoverage::fromRequestString(urlSplit.at(1), RequestBase::WCS);
            }
            else if(QString::compare(serviceType.cap(1), "ovp", Qt::CaseInsensitive) == 0
                    && QString::compare(requestType.cap(1), "GetCoverage", Qt::CaseInsensitive) == 0)
            {
                request = GetCoverage::fromRequestString(urlSplit.at(1), RequestBase::OVP);
            }
            else if(QString::compare(serviceType.cap(1), "wms", Qt::CaseInsensitive) == 0
               && QString::compare(requestType.cap(1), "GetMap", Qt::CaseInsensitive) == 0)
            {
                request = GetMap::fromRequestString(urlSplit.at(1));
            }
            else if(QString::compare(serviceType.cap(1), "ovp", Qt::CaseInsensitive) == 0
                    && QString::compare(requestType.cap(1), "DescribeCoverages", Qt::CaseInsensitive) == 0)
            {
                request = DescribeCoverages::fromRequestString(urlSplit.at(1));
            }
        }
        else if(tokens[0] == "POST") {
            qDebug() << "POST not supported yet.";
            return;
        }
        else {
            return;
        }
        
        handleRequest(socket, request);
        
        delete request;
    }
}

void OceanVisServer::handleRequest(QTcpSocket *socket, RequestBase *request)
{
    if(!request) {
        qDebug() << "Request is empty.";
    }
    GetCoverage *getCoverage = dynamic_cast<GetCoverage *>(request);
    GetMap *getMap = dynamic_cast<GetMap *>(request);
    DescribeCoverages *describeCoverages = dynamic_cast<DescribeCoverages *>(request);

    if(getCoverage) {
        handleGetCoverage(socket, getCoverage);
    }
    else if(getMap) {
        handleGetMap(socket, getMap);
    }
    else if(describeCoverages) {
        handleDescribeCoverages(socket, describeCoverages);
    }
    else {
        qDebug() << "Unknown request.";
    }
    
    if(socket->state() == QTcpSocket::UnconnectedState) {
        delete socket;
        qDebug() << "Connection closed";
    }
}

void OceanVisServer::handleGetCoverage(QTcpSocket *socket, GetCoverage *getCoverage)
{
    if(getCoverage->requestType() == RequestBase::WCS
       && getCoverage->version() != "2.0.0")
    {
        // TODO: This is the wrong behavior.
        qDebug() << "WCS: Wrong version.";
        return;
    }
    if(getCoverage->requestType() == RequestBase::OVP
       && getCoverage->version() != "1.0.0")
    {
        wrongOvpVersion(socket);
        return;
    }

    DataLayer *selectedLayer;
    QHash<QString,DataLayer *>::const_iterator layer = m_layers.constFind(getCoverage->coverageId());
    if(layer == m_layers.constEnd()
       && getCoverage->requestType() == RequestBase::WCS)
    {
        // TODO: This is the wrong behavior.
        qDebug() << "Coverage not found.";
        return;
    }
    else if(layer == m_layers.constEnd()
            && getCoverage->requestType() == RequestBase::OVP)
    {
        badOvpCoverageId(socket, getCoverage->coverageId());
        return;
    }
    else {
        selectedLayer = layer.value();
    }
    
    QList<DimensionSubset *> dimensionSubsets = getCoverage->dimensionSubsets();
    qDebug() << "Number of subset things:" << dimensionSubsets.length();
    GridCoverage *matrix = 0;
    try {
        matrix = selectedLayer->dataSubset(dimensionSubsets, getCoverage->cutMode());
    } catch (BadSlicePosition e) {        
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        
        ResponseWriter writer;
        writer.setDevice(&buffer);
        writer.write(e);
        buffer.close();
        
        sendHtmlOkData(socket, buffer.buffer(), "text/xml; charset=\"utf-8\"");
    }
    
    if(!matrix) {
        socket->close();
        return;
    }
    
    if(getCoverage->format() == "text/xml") {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        ResponseWriter writer;
        writer.setDevice(&buffer);
        writer.write(matrix);
        buffer.close();
        
        sendHtmlOkData(socket, buffer.buffer(), "text/xml; charset=\"utf-8\"");
    }
    else if(getCoverage->format().startsWith("image/")) {
        QString format = getCoverage->format().remove(0,6);
        qDebug() << "Got image, the format is" << format;
        
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        QImageWriter iw;
        iw.setDevice(&buffer);
        iw.setFormat(format.toAscii());
        iw.write(matrix->toImage(getCoverage->sizes()));
        buffer.close();
        
        sendHtmlOkData(socket, buffer.buffer(), "image/" + format);
    }
     
    socket->close();
    
    delete matrix;
}

void OceanVisServer::handleGetMap(QTcpSocket *socket, GetMap *getMap)
{
    if(getMap->version() != "1.3.0"
       && getMap->version() != "1.1.1"
       && getMap->version() != "1.1.0")
    {
        // TODO: This is the wrong behavior.
        qDebug() << "WMS: Wrong version" << getMap->version();
        return;
    }

    QStringList layers = getMap->layers();
    QList<DataLayer *> selectedLayers;
    for(int i = 0; i < layers.size(); ++i) {
        QHash<QString,DataLayer *>::const_iterator layer = m_layers.constFind(layers[i]);
        if(layer == m_layers.constEnd()) {
            // TODO: This is the wrong behavior.
            qDebug() << "Coverage not found.";
            return;
        }
        else {
            selectedLayers.append(layer.value());
        }
    }
    
    QImage imageResult(getMap->width(), getMap->height(), QImage::Format_ARGB32);
    if(getMap->transparent()) {
        imageResult.fill(qRgba(0,0,0,0));
    }
    else {
        imageResult.fill(getMap->backgroundColor().rgb());
    }
    QPainter painter(&imageResult);
    
    QList<DimensionSubset *> dimensionSubsets = getMap->dimensionSubsets();
    BoundingBox boundingBox = getMap->boundingBox();
    CoordinateAxis xAxis(Lon), yAxis(Lat);
    xAxis.setLowerLimit(boundingBox.lonMin());
    xAxis.setUpperLimit(boundingBox.lonMax());
    xAxis.setValueCount(getMap->width());
    yAxis.setLowerLimit(boundingBox.latMin());
    yAxis.setUpperLimit(boundingBox.latMax());
    yAxis.setValueCount(getMap->height());

    qDebug() << "Number of subset things:" << dimensionSubsets.length();
    for(int i = 0; i < selectedLayers.size(); ++i) {
        GridCoverage *matrix = selectedLayers.at(i)->dataSubset(dimensionSubsets, DataLayer::Overlaps);
        if(!matrix) {
            // TODO: Still the wrong behavior.
            return;
        }
        
        QImage image = matrix->toImage(xAxis, yAxis, true);
        image.save("/home/bastian/test.png");
        painter.drawImage(0, 0, image);
        
        delete matrix;
    }
    
    QString format = getMap->format().remove(0,6);
    qDebug() << "Got image, the format is" << format;
    
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QImageWriter iw;
    iw.setDevice(&buffer);
    iw.setFormat(format.toAscii());
    iw.write(imageResult);
    buffer.close();
    
    sendHtmlOkData(socket, buffer.buffer(), "image/" + format);
    
    socket->close();
}

void OceanVisServer::handleDescribeCoverages(QTcpSocket *socket, DescribeCoverages *describeCoverages)
{
    if(describeCoverages->version() != "1.0.0")
    {
        wrongOvpVersion(socket);
        return;
    }
    
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    ResponseWriter writer;
    writer.setDevice(&buffer);
    writer.writeCoverages(m_layers);
    buffer.close();
    
    sendHtmlOkData(socket, buffer.buffer(), "text/xml; charset=\"utf-8\"");
    
    socket->close();
}

void OceanVisServer::wrongOvpVersion(QTcpSocket *socket)
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    
    ResponseWriter writer;
    writer.setDevice(&buffer);
    writer.writeWrongOvpVersion();
    buffer.close();
    
    sendHtmlOkData(socket, buffer.buffer(), "text/xml; charset=\"utf-8\"");
    
    socket->close();
}

void OceanVisServer::badOvpCoverageId(QTcpSocket *socket, const QString& givenCoverageId)
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    
    ResponseWriter writer;
    writer.setDevice(&buffer);
    writer.writeBadCoverageId(givenCoverageId, m_layers);
    buffer.close();
    
    sendHtmlOkData(socket, buffer.buffer(), "text/xml; charset=\"utf-8\"");
}

void OceanVisServer::discardClient()
{
    QTcpSocket *socket = (QTcpSocket *) sender();
    socket->deleteLater();

    qDebug() << "Connection closed";
}

void OceanVisServer::sendHtmlOkData(QTcpSocket *socket,
                                    QByteArray data,
                                    QString contentType)
{
    sendHtmlOkHeader(socket, data.size(), contentType);
    socket->write(data);
}

void OceanVisServer::sendHtmlOkHeader(QTcpSocket *socket,
                                      int contentLength,
                                      QString contentType)
{
    QLocale locale(QLocale::C);
    
    QTextStream os(socket);
    os.setAutoDetectUnicode(true);
    os << "HTTP/1.1 200 Ok\r\n";
    os << "Connection: close\r\n";
    os << "Date: " << locale.toString(QDateTime::currentDateTime().toUTC(),
                                      "ddd, dd MMM yyyy hh:mm:ss 'GMT'") << "\r\n";
    os << "Accept-Ranges: none\r\n";
    os << "Server: OceanVisServer/1.0.0\r\n";
    os << "Content-Length: " << QString::number(contentLength) << "\r\n";
    os << "Content-Type: " << contentType << "\r\n";
    os << "Last-Modified: " << locale.toString(m_lastModified,
                                               "ddd, dd MMM yyyy hh:mm:ss 'GMT'") << "\r\n";
    os << "\r\n";
    os.flush();
}

void OceanVisServer::sendHtmlBadRequest(QTcpSocket *socket)
{
    QLocale locale(QLocale::C);
    
    QTextStream os(socket);
    os.setAutoDetectUnicode(true);
    os << "HTTP/1.1 400 Bad Request\r\n";
    os << "Connection: close\r\n";
    os << "Date: " << locale.toString(QDateTime::currentDateTime().toUTC(),
                                      "ddd, dd MMM yyyy hh:mm:ss 'GMT'") << "\r\n";
    os << "Accept-Ranges: none\r\n";
    os << "Server: OceanVisServer/1.0.0\r\n";
    os << "\r\n";
    os.flush();
}

#include "OceanVisServer.moc"
