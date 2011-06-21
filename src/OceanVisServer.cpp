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

// Project
#include "DataLayer.h"

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
        QString request;

        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if(tokens[0] == "GET") {
            QString url = tokens[1];
            QStringList urlSplit = url.split('?');
            if(urlSplit.size() < 2) {
                qDebug() << "Invalid request";
                return;
            }

            QStringList args = urlSplit.at(1).split('&');

            QString service;
            bool tiff = false;
            foreach(QString arg, args) {
                QStringList argParts = arg.split('=');
                if(argParts.size() != 2) {
                    qDebug() << "Strange arguments.";
                    continue;
                }

                if(argParts[0] == "SERVICE") {
                    service = argParts[1];
                }
                else if(argParts[0] == "VERSION") {
                    if(argParts[1] != "2.0.0") {
                        // TODO: This is the wrong behavior.
                        qDebug() << "Wrong version.";
                        return;
                    }
                }
                else if(argParts[0] == "REQUEST") {
                    request = argParts[1];
                }
                else if(argParts[0] == "COVERAGEID") {
                    coverages = argParts[1].split(',');
                }
                else if(argParts[0] == "FORMAT") {
                    if(argParts[1] == "image/tiff") {
                        tiff = true;
                    }
                }
                else if(argParts[0] == "SUBSET") {
                    qDebug() << "Parsing subset.";
                    QString arg = argParts[1];
                    QRegExp exp("(lon|lat),http://www.opengis.net/def/crs/EPSG/0/4326\\(([-+]?[0-9]{0,}\\.?[0-9]{0,}),([-+]?[0-9]{0,}\\.?[0-9]*)\\)");
                    exp.indexIn(arg);
                    if(exp.pos() > -1) {
                        QString axis = exp.cap(1);
                        QString min = exp.cap(2);
                        QString max = exp.cap(3);
                        
                        qDebug() << "Axis:" << axis;
                        qDebug() << "Min:" << min;
                        qDebug() << "Max:" << max;
                    }
                }
            }

            if(service != "wcs") {
                qDebug() << "Wrong service";
                return;
            }

            if(request.isEmpty()) {
                qDebug() << "No request.";
                return;
            }
        }
        else if(tokens[0] == "POST") {
            qDebug() << "POST not supported yet.";
            return;
        }
        else {
            return;
        }

        QList<DataLayer *> selectedLayers;
        foreach(QString coverage, coverages) {
            QHash<QString,DataLayer *>::const_iterator layer = m_layers.constFind(coverage);
            if(layer == m_layers.constEnd()) {
                qDebug() << "Coverage not found.";
                return;
            }
            else {
                selectedLayers.append(layer.value());
            }
        }

        QTextStream os(socket);
        os.setAutoDetectUnicode(true);
        os << "HTTP/1.0 200 Ok\r\n"
              "Content-Type: text/html; charset=\"utf-8\"\r\n"
              "\r\n"
              "<h1>" + request + "</h1>\n"
              "<p>Selected " + QString::number(selectedLayers.size()) + " layers.</p>"
           << QDateTime::currentDateTime().toString() << "\n";
        socket->close();

        qDebug() << "Wrote to client";

        if(socket->state() == QTcpSocket::UnconnectedState) {
            delete socket;
            qDebug() << "Connection closed";
        }
    }
}

void OceanVisServer::discardClient()
{
    QTcpSocket *socket = (QTcpSocket *) sender();
    socket->deleteLater();

    qDebug() << "Connection closed";
}

#include "OceanVisServer.moc"
