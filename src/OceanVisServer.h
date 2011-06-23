//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef OCEANVISSERVER_H
#define OCEANVISSERVER_H

// Qt
#include <QtCore/QHash>
#include <QtNetwork/QTcpServer>

class DataLayer;
class QTcpSocket;
class RequestBase;
class GetCoverage;

class OceanVisServer : public QTcpServer
{
Q_OBJECT
public:
    OceanVisServer(QObject *parent = 0);
    virtual ~OceanVisServer();

    void incomingConnection(int socket);

    void pause();
    void resume();

    /**
     * Set the layers the server has to serve.
     * This object will take the ownership of each layer.
     */
    void setLayers(const QHash<QString,DataLayer *>& layers);

    /**
     * Return the layers of the server.
     */
    QHash<QString,DataLayer *> layers() const;
    
    /**
     * Answer the given request using the given socket.
     */
    void handleRequest(QTcpSocket *socket, RequestBase *request);
    
    /**
     * Answer the getCoverage request.
     * Sends the requested coverage or, if this is not possible, an error
     * message through the given socket.
     */
    void handleGetCoverage(QTcpSocket *socket, GetCoverage *getCoverage);

private slots:
    void readClient();
    void discardClient();

private:
    bool disabled;
    QHash<QString,DataLayer *> m_layers;
};

#endif
