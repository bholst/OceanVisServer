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

// Project
#include "DataLayer.h"
#include "RequestBase.h"
#include "GetCoverage.h"
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
        QStringList coverages;
        
        RequestBase *request = 0;

        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if(tokens[0] == "GET") {
            QString url = tokens[1];
            QStringList urlSplit = url.split('?');
            if(urlSplit.size() < 2) {
                qDebug() << "Invalid request";
                return;
            }
            
            QRegExp serviceType("SERVICE=(wcs)");
            serviceType.indexIn(urlSplit.at(1));
            QRegExp requestType("REQUEST=(GetCoverage)");
            requestType.indexIn(urlSplit.at(1));

            if(serviceType.pos() <= -1) {
                return;
            }
            if(serviceType.cap(1) == "wcs"
               && requestType.cap(1) == "GetCoverage")
            {
                request = new GetCoverage();
            }
            else {
                return;
            }

            QStringList args = urlSplit.at(1).split('&');

            foreach(QString arg, args) {
                QStringList argParts = arg.split('=');
                if(argParts.size() != 2) {
                    qDebug() << "Strange arguments.";
                    continue;
                }

                else if(argParts[0] == "VERSION") {
                    request->setVersion(argParts[1]);
                }
                else if(argParts[0] == "COVERAGEID") {
                    GetCoverage *getCoverage = dynamic_cast<GetCoverage *>(request);
                    if(getCoverage) {
                        getCoverage->setCoverageId(argParts[1]);
                    }
                }
                else if(argParts[0] == "FORMAT") {
                    GetCoverage *getCoverage = dynamic_cast<GetCoverage *>(request);
                    if(getCoverage) {
                        getCoverage->setFormat(argParts[1]);
                    }
                }
                else if(argParts[0] == "SUBSET"
                        || argParts[0] == "TRIM"
                        || argParts[0] == "SLICE")
                {
                    GetCoverage *getCoverage = dynamic_cast<GetCoverage *>(request);
                    if(!getCoverage) {
                        qDebug() << "No get coverage.";
                    }
                    else {
                        qDebug() << "Parsing subset.";
                        QString arg = argParts[1];
                        QRegExp trimExp("([a-zA-z]{1,}),http://www.opengis.net/def/crs/EPSG/0/4326\\(([-+]?[0-9]{0,}\\.?[0-9]{0,}),([-+]?[0-9]{0,}\\.?[0-9]*)\\)");
                        trimExp.indexIn(arg);
                        if(trimExp.pos() > -1) {
                            qDebug() << "Found trim.";
                            QString axis = trimExp.cap(1);
                            QString min = trimExp.cap(2);
                            QString max = trimExp.cap(3);
                            
                            qDebug() << "Axis:" << axis;
                            qDebug() << "Min:" << min;
                            qDebug() << "Max:" << max;
                            
                            try {
                                DimensionTrim *trim = new DimensionTrim(axis);
                                
                                if(trim->dimension() == Time) {
                                    trim->setTrimLow(RequestParser::parseTime(min));
                                    trim->setTrimHigh(RequestParser::parseTime(max));
                                }
                                else {
                                    trim->setTrimLow(min.toDouble());
                                    trim->setTrimHigh(max.toDouble());
                                }
                                getCoverage->addDimensionSubset(trim);
                            } catch (BadDimensionString e) {
                                qDebug() << e.what();
                                delete request;
                                return;
                            }
                        }
                        else {
                            QRegExp sliceExp("([a-zA-z]{1,}),http://www.opengis.net/def/crs/EPSG/0/4326\\((\\S{1,})\\)");
                            sliceExp.indexIn(arg);
                            if(sliceExp.pos() > -1) {
                                qDebug() << "Fount slice";
                                QString axis = sliceExp.cap(1);
                                QString slicePointString = sliceExp.cap(2);
                                
                                qDebug() << "Axis:" << axis;
                                qDebug() << "Slice point:" << slicePointString;
                                
                                try {
                                    DimensionSlice *slice = new DimensionSlice(axis);
                                    
                                    if(slice->dimension() == Time) {
                                        slice->setSlicePoint(QVariant(RequestParser::parseTime(slicePointString)));
                                    }
                                    else {
                                        slice->setSlicePoint(slicePointString.toDouble());
                                    }
                                    getCoverage->addDimensionSubset(slice);
                                } catch (BadDimensionString e) {
                                    qDebug() << e.what();
                                    delete request;
                                    return;
                                }
                            }
                        }
                    }
                }
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
    if(request->version() != "2.0.0") {
        // TODO: This is the wrong behavior.
        qDebug() << "Wrong version.";
        return;
    }

    GetCoverage *getCoverage = dynamic_cast<GetCoverage *>(request);
    if(getCoverage) {
        handleGetCoverage(socket, getCoverage);
    }
    else {
        qDebug() << "Unknown request.";
    }
}

void OceanVisServer::handleGetCoverage(QTcpSocket *socket, GetCoverage *getCoverage)
{    
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
        iw.write(matrix->toImage());
    }
     
    socket->close();
    
    delete matrix;
    
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
