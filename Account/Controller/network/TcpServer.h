#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include "../core/abstractcontroller.h"
#include "AccountSocket.h"
#include <QTcpServer>
#include <QtCore/qglobal.h>

class TcpServer : public QTcpServer
{
public:
    TcpServer() = default;

protected:
    void incomingConnection(qintptr);
};

#endif // TCPSERVER_HPP
