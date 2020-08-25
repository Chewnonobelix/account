#include "controllersynchronization.h"

int ControllerSynchronization::exec()
{
    //    m_server.listen(QHostAddress::Any, 6000);

    connect(&m_server, &QTcpServer::newConnection, this, &ControllerSynchronization::newConnections);

    connect(&m_broadcast,
            &QUdpSocket::readyRead,
            this,
            &ControllerSynchronization::receivedDatagram);

    startTimer(60000);
    return 0;
}

void ControllerSynchronization::newConnections()
{
    m_connections<<AccountSocket();
    m_connections.last().setSocket(m_server.nextPendingConnection());
}

AccountSocket::AccountSocket(const AccountSocket & as): AbstractController(),
    m_socket(as.m_socket)
{}

AccountSocket::~AccountSocket()
{
    delete m_socket;
}

void AccountSocket::setSocket(QTcpSocket * as)
{
    if(m_socket)
        disconnect(m_socket, &QIODevice::readyRead, this, &AccountSocket::receiveDataSocket);
    
    m_socket = as;
    
    connect(m_socket, &QIODevice::readyRead, this, &AccountSocket::receiveDataSocket);
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

int AccountSocket::exec()
{
    return 0;
}

void ControllerSynchronization::timerEvent(QTimerEvent *)
{
    if (m_server.isListening())
        lookup();
}

void ControllerSynchronization::lookup()
{
    m_broadcast.writeDatagram("account_server:test_server", QHostAddress(QHostAddress::Broadcast), 7000);
}

void ControllerSynchronization::sync()
{
    m_client.sync();
    for(auto it: m_connections)
        it.sync();
}

void AccountSocket::sync()
{
    qDebug()<<"Sync";
}

void ControllerSynchronization::receivedDatagram()
{
    QByteArray data;
    QHostAddress addr;

    while(m_broadcast.hasPendingDatagrams())
    {
        data.resize(m_broadcast.pendingDatagramSize());
        m_broadcast.readDatagram(data.data(), m_broadcast.pendingDatagramSize(), &addr);
        auto split = data.split(':');

        if(split[0] != "account_server" && !m_server.isListening())
            continue;

        clientConnect(addr);
    }
}

void ControllerSynchronization::clientConnect(QHostAddress addr)
{
    if(!m_server.isListening())
        m_client.connectTo(addr);
}

void AccountSocket::connectTo(QHostAddress addr)
{
    if (!m_socket)
        m_socket = new QTcpSocket;

    m_socket->connectToHost(addr, 7000);
}

void ControllerSynchronization::openServer(bool isOpen)
{
    if (isOpen && !m_server.isListening()) {
        m_client.close();
        m_server.listen(QHostAddress::Any, 9000);
    } else if (!isOpen) {
        m_server.close();
    }
}

void AccountSocket::close()
{
    m_socket->close();
}
