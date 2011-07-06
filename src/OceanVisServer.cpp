//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

// Standard
#include <iostream>

// Qt
#include <QtAlgorithms>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <QtCore/QRegExp>
#include <QtCore/QBuffer>
#include <QtCore/QDataStream>
#include <QtGui/QImageWriter>
#include <QtGui/QImage>
#include <QtGui/QPainter>

// Project
#include "Dimension.h"
#include "DataLayer.h"
#include "RequestBase.h"
#include "GetCoverage.h"
#include "GetMap.h"
#include "DimensionTrim.h"
#include "DimensionSlice.h"
#include "GridCoverage.h"
#include "RequestParser.h"
#include "ResponseWriter.h"

// Self
#include "OceanVisServer.h"

OceanVisServer::OceanVisServer(QObject *parent)
    : disabled(false)
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
            
            QRegExp serviceType("SERVICE=(wcs|wms)");
            serviceType.indexIn(urlSplit.at(1));
            QRegExp requestType("REQUEST=(GetMap|GetCoverage)");
            requestType.indexIn(urlSplit.at(1));

            if(serviceType.pos() <= -1) {
                return;
            }
            if(serviceType.cap(1) == "wcs"
               && requestType.cap(1) == "GetCoverage")
            {
                request = GetCoverage::fromRequestString(urlSplit.at(1));
            }
            else if(serviceType.cap(1) == "wms"
               && requestType.cap(1) == "GetMap")
            {
                request = GetMap::fromRequestString(urlSplit.at(1));
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
    if(getCoverage) {
        handleGetCoverage(socket, getCoverage);
    }
    else if(getMap) {
        handleGetMap(socket, getMap);
    }
    else {
        qDebug() << "Unknown request.";
    }
}

void OceanVisServer::handleGetCoverage(QTcpSocket *socket, GetCoverage *getCoverage)
{
    if(getCoverage->version() != "2.0.0") {
        // TODO: This is the wrong behavior.
        qDebug() << "WCS: Wrong version.";
        return;
    }

    DataLayer *selectedLayer;
    QHash<QString,DataLayer *>::const_iterator layer = m_layers.constFind(getCoverage->coverageId());
    if(layer == m_layers.constEnd()) {
        // TODO: This is the wrong behavior.
        qDebug() << "Coverage not found.";
        return;
    }
    else {
        selectedLayer = layer.value();
    }
    
    QList<DimensionSubset *> dimensionSubsets = getCoverage->dimensionSubsets();
    qDebug() << "Number of subset things:" << dimensionSubsets.length();
    GridCoverage *matrix = selectedLayer->dataSubset(dimensionSubsets);
    if(!matrix) {
        return;
    }
    
    QTextStream os(socket);
    os.setAutoDetectUnicode(true);
    os << "HTTP/1.0 200 Ok\r\n";
    
    if(getCoverage->format() == "text/xml") {
        os << "Content-Type: text/xml; charset=\"utf-8\"\r\n"
              "\r\n";
        os.flush();
        
        ResponseWriter writer;
        writer.setDevice(socket);
        writer.write(matrix);
    }
    else if(getCoverage->format().startsWith("image/")) {
        qDebug() << "Got image";
        QString format = getCoverage->format().remove(0,6);
        qDebug() << "The format is" << format;
        os << "Content-Type: image/" + format + "\r\n"
           << "\r\n";
        os.flush();
        qDebug() << "Hej an image";
        QImageWriter iw;
        iw.setDevice(socket);
        iw.setFormat(format.toAscii());
        iw.write(matrix->toImage(getCoverage->sizes()));
    }
     
    socket->close();
    
    delete matrix;
    
    if(socket->state() == QTcpSocket::UnconnectedState) {
        delete socket;
        qDebug() << "Connection closed";
    }
}

void OceanVisServer::handleGetMap(QTcpSocket *socket, GetMap *getMap)
{
    if(getMap->version() != "1.3.0") {
        // TODO: This is the wrong behavior.
        qDebug() << "WMS: Wrong version.";
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
    qDebug() << "Number of subset things:" << dimensionSubsets.length();
    for(int i = 0; i < selectedLayers.size(); ++i) {
        GridCoverage *matrix = selectedLayers.at(i)->dataSubset(dimensionSubsets);
        if(!matrix) {
            // TODO: Still the wrong behavior.
            return;
        }
        
        QImage image = matrix->toImage(QSize(getMap->width(), getMap->height()), true);
        painter.drawImage(0, 0, image);
        
        delete matrix;
    }
    
    QTextStream os(socket);
    os.setAutoDetectUnicode(true);
    os << "HTTP/1.0 200 Ok\r\n";
    
    qDebug() << "Got image";
    QString format = getMap->format().remove(0,6);
    qDebug() << "The format is" << format;
    os << "Content-Type: image/" + format + "\r\n"
       << "\r\n";
    os.flush();
    
    QImageWriter iw;
    iw.setDevice(socket);
    iw.setFormat(format.toAscii());
    iw.write(imageResult);
    
    socket->close();
    
    if(socket->state() == QTcpSocket::UnconnectedState) {
        delete socket;
        qDebug() << "Connection closed";
    }
}

void OceanVisServer::discardClient()
{
    QTcpSocket *socket = (QTcpSocket *) sender();
    socket->deleteLater();

    qDebug() << "Connection closed";
}

#include "OceanVisServer.moc"
