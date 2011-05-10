#include <QCoreApplication>
#include "OceanVisServer.h"


int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    OceanVisServer foo;
    return app.exec();
}
