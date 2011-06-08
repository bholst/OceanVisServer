//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef OCEANVISSERVER_H
#define OCEANVISSERVER_H

// Qt
#include <QtCore/QList>
#include <QtNetwork/QTcpServer>

class DataLayer;

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
    void setLayers(const QList<DataLayer *>& layers);

    /**
     * Return the layers of the server.
     */
    QList<DataLayer *> layers() const;

private slots:
    void readClient();
    void discardClient();

private:
    bool disabled;
    QList<DataLayer *> m_layers;
};

#endif
