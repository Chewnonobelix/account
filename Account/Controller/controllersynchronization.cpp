#include "controllersynchronization.h"

int ControllerSynchronization::exec()
{
    connect(&m_server, &QTcpServer::newConnection, this, &ControllerSynchronization::newConnections);

    connect(&m_broadcast,
            &QUdpSocket::readyRead,
            this,
            &ControllerSynchronization::receivedDatagram);

    startTimer(5000);

    return 0;
}

void ControllerSynchronization::setView(QObject *v)
{
    m_view = v;
}

void ControllerSynchronization::newConnections()
{
    m_connections<<AccountSocket();
    m_connections.last().setSocket(m_server.nextPendingConnection());
}

void ControllerSynchronization::timerEvent(QTimerEvent *)
{
    if (m_server.isListening())
        lookup();
}

void ControllerSynchronization::lookup()
{
    qDebug() << "Lookup" << m_server.isListening();
    if (m_server.isListening())
        m_broadcast.writeDatagram("account_server_connect:test_server",
                                  QHostAddress(QHostAddress::Broadcast),
                                  7000);
}

void ControllerSynchronization::sync()
{
    m_client.sync();
    for(auto it: m_connections)
        it.sync();
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

void ControllerSynchronization::openServer(bool isOpen)
{
    if (isOpen && !m_server.isListening()) {
        m_client.close();
        m_server.listen(QHostAddress(QHostAddress::AnyIPv4), 9000);
    } else if (!isOpen) {
        m_server.close();
    }
}

QString AccountSocket::remoteName() const
{
    return m_remoteName;
}

AccountSocket::AccountSocket(const AccountSocket &as) : AbstractController(), m_socket(as.m_socket)
{}

AccountSocket::~AccountSocket()
{
    delete m_socket;
}

void AccountSocket::setSocket(QTcpSocket *as)
{
    if (m_socket)
        disconnect(m_socket, &QIODevice::readyRead, this, &AccountSocket::receiveDataSocket);

    m_socket = as;

    connect(m_socket, &QIODevice::readyRead, this, &AccountSocket::receiveDataSocket);

    getProfileid();
    getRemotename();

    qDebug() << "New connection from remote name" << remoteName() << m_socket->peerAddress();
}

void AccountSocket::receiveDataSocket()
{
    auto d = m_socket->readAll();
    parser(d);
}

AccountSocket &AccountSocket::operator=(const AccountSocket &as)
{
    setSocket(as.m_socket);
    return *this;
}

int AccountSocket::exec()
{
    return 0;
}

void AccountSocket::close()
{
    if (m_socket)
        m_socket->close();
}

void AccountSocket::sync()
{
    qDebug() << "Sync";
}

void AccountSocket::connectTo(QHostAddress addr)
{
    if (!m_socket)
        m_socket = new QTcpSocket;

    m_socket->connectToHost(addr, 7000);
}

bool AccountSocket::isConnected() const
{
    return m_socket && m_socket->isOpen();
}

void AccountSocket::parser(QString data)
{
    auto split = data.split(":");
    qDebug() << "Parse" << data;
    if (split[0] != "account_api")
        return;

    if (split[1] == "post") {
        if (split[2] == "localName")
            m_remoteName = split[3];
        if (split[2] == "syncId")
            remoteProfile.setId(QUuid::fromString(split[3]));
    }

    if (split[1] == "get") {
        if (split[2] == "localName")
            postLocalname();
        if (split[2] == "syncId")
            postProfileid();
    }
}

SynchronizationProfile AccountSocket::profile(QString remote) const
{
    auto list = m_db->selectSyncProfile();
    SynchronizationProfile ret;

    for (auto it : list) {
        if ((it.deviceName() == remote || it.deviceName() == QHostInfo::localHostName())
            && (it.hostName() == remote || it.hostName() == QHostInfo::localHostName())) {
            ret = it;
        }
    }
}

void AccountSocket::postLocalname()
{
    if (isConnected())
        m_socket->write("account_api:post:localName:" + QHostInfo::localHostName().toLatin1());
}

void AccountSocket::getRemotename()
{
    if (isConnected())
        qDebug() << "Write" << m_socket->write("account_api:get:localName");
}

void AccountSocket::postProfileid()
{
    auto id = profile(remoteName()).id();

    if (isConnected())
        m_socket->write("account_api:post:syncId:" + id.toByteArray());
}

void AccountSocket::getProfileid()
{
    if (isConnected())
        m_socket->write("account_api:get:syncId");
}

void AccountSocket::postProfile() {}
void AccountSocket::getProfile()
{
    if (isConnected())
        m_socket->write("account_api:get:syncProfile");
}
