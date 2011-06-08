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

void OceanVisServer::setLayers(const QList<DataLayer *>& layers)
{
    m_layers = layers;
}

QList<DataLayer *> OceanVisServer::layers() const
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
        if(tokens[0] == "GET") {
            QTextStream os(socket);
            os.setAutoDetectUnicode(true);
            os << "HTTP/1.0 200 Ok\r\n"
                  "Content-Type: text/html; charset=\"utf-8\"\r\n"
                  "\r\n"
                  "<h1>Nothing to see here</h1>\n"
               << QDateTime::currentDateTime().toString() << "\n";
            socket->close();

            qDebug() << "Wrote to client";

            if(socket->state() == QTcpSocket::UnconnectedState) {
                delete socket;
                qDebug() << "Connection closed";
            }
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
