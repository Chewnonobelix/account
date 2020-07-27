#include "controllersynchronization.h"

int ControllerSynchronization::exec()
{
    connect(&m_server, QTcpServer::newConnection, this, ControllerSynchronization::newConnections);
}

void ControllerSynchronization::newConnections()
{
    m_connections<<m_server.nextPendingConnection()   
}
