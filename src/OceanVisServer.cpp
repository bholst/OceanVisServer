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
        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if(tokens[0] != "GET") {
            return;
        }

        QString url = tokens[1];
        QStringList urlSplit = url.split('?');
        if(urlSplit.size() < 2) {
            qDebug() << "Invalid request";
            return;
        }

        QStringList args = urlSplit.at(1).split('&');

        QString service;
        foreach(QString arg, args) {
            QStringList argParts = arg.split('=');
            if(argParts.size() != 2) {
                qDebug() << "Strange arguments.";
                continue;
            }

            if(argParts[0] == "SERVICE") {
                service = argParts[1];
            }
        }

        if(service != "wcs") {
            return;
        }

        QTextStream os(socket);
        os.setAutoDetectUnicode(true);
        os << "HTTP/1.0 200 Ok\r\n"
              "Content-Type: text/html; charset=\"utf-8\"\r\n"
              "\r\n"
              "<h1>Nothing to see here, yet</h1>\n"
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
