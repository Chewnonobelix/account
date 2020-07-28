#include "controllersynchronization.h"

int ControllerSynchronization::exec()
{
    m_server.listen(QHostAddress::Any, 6000);
    
    connect(&m_server, QTcpServer::newConnection, this, ControllerSynchronization::newConnections);
    return 0;
}

void ControllerSynchronization::newConnections()
{
    m_connections<<AccountSocket();
    m_connections.last().setSocket(m_server.nextPendingConnection());
}

AccountSocket::AccountSocket(const AccountSocket & as): QObject(nullptr),
    m_socket(as.m_socket)
{}

AccountSocket::~AccountSocket()
{
    delete m_socket;
}

void AccountSocket::setSocket(QTcpSocket * as)
{
    if(m_socket)
        disconnect(m_socket, QIODevice::readyRead, this, AccountSocket::receiveDataSocket);
    
    m_socket = as;
    
    connect(m_socket, QIODevice::readyRead, this, AccountSocket::receiveDataSocket);
}

void AccountSocket::receiveDataSocket()
{
    auto d = m_socket->readAll();
}

AccountSocket& AccountSocket::operator =(const AccountSocket& as)
{
    setSocket(as.m_socket);
    return *this;
}
