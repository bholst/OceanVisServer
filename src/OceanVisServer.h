//
// Copyright 2011      Bastian Holst <bastianholst@gmx.de>
//

#ifndef OCEANVISSERVER_H
#define OCEANVISSERVER_H

#include <QtNetwork/QTcpServer>

class OceanVisServer : public QTcpServer
{
Q_OBJECT
public:
    OceanVisServer(quint16 port, QObject *parent = 0);
    virtual ~OceanVisServer();

    void incomingConnection(int socket);

    void pause();
    void resume();

private slots:
    void readClient();
    void discardClient();

private:
    bool disabled;
};

#endif
