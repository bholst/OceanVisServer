#ifndef OCEANVISSERVER_H
#define OCEANVISSERVER_H

#include <QtCore/QObject>

class OceanVisServer : public QObject
{
Q_OBJECT
public:
    OceanVisServer();
    virtual ~OceanVisServer();
private slots:
    void output();
};

#endif // OCEANVISSERVER_H
