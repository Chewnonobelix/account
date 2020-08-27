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

    connect(&m_connections.last(),
            &AccountSocket::disconnected,
            this,
            &ControllerSynchronization::onDisconnected);

    updateViewList();
}

void ControllerSynchronization::updateViewList()
{
    auto *list = m_view->findChild<QObject *>("syncProfiles");
    QVariantList vl;

    for (auto &it : m_connections)
        vl << QVariant::fromValue(&it);

    list->setProperty("model", vl);
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

void ControllerSynchronization::onDisconnected()
{
    auto *send = dynamic_cast<AccountSocket *>(sender());
    m_connections.removeAll(*send);

    updateViewList();
}

QString AccountSocket::remoteName() const
{
    return m_remoteName;
}

AccountSocket::AccountSocket(const AccountSocket &as) : AbstractController(), m_socket(as.m_socket)
{}

AccountSocket::~AccountSocket()
{
    close();
}

void AccountSocket::setSocket(QTcpSocket *as)
{
    if (m_socket)
        disconnect(m_socket, &QIODevice::readyRead, this, &AccountSocket::receiveDataSocket);

    close();
    m_client = false;
    m_socket = as;

    connect(m_socket, &QIODevice::readyRead, this, &AccountSocket::receiveDataSocket);
    connect(m_socket, &QTcpSocket::disconnected, this, &AccountSocket::disconnected);

    getProfileid();
    getRemotename();

    qDebug() << "New connection from remote name" << m_socket->peerName() << remoteName()
             << m_socket->peerAddress();
}

void AccountSocket::receiveDataSocket()
{
    if (!m_socket)
        return;

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
    if (m_socket && m_client) {
        m_socket->close();
        delete m_socket;
        m_socket = nullptr;
    }
}

void AccountSocket::sync()
{
    qDebug() << "Sync";
}

void AccountSocket::connectTo(QHostAddress addr)
{
    if (!m_socket)
        m_socket = new QTcpSocket;

    m_client = true;
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
        if (split[2] == "localName") {
            m_remoteName = split[3];
            m_localProfile = profile(m_remoteName);

            emit remoteNameChanged(m_remoteName);
        }
        if (split[2] == "syncId") {
            m_remoteProfile.setId(QUuid::fromString(split[3]));
        }
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

    return ret;
}

SynchronizationProfile AccountSocket::profile() const
{
    return m_localProfile;
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

bool operator==(const AccountSocket &as1, const AccountSocket &as2)
{
    return as1.m_socket == as2.m_socket;
}
