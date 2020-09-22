#include "controllersynchronization.h"


int ControllerSynchronization::exec()
{
    connect(&m_server, &QTcpServer::newConnection, this, &ControllerSynchronization::newConnections);

    connect(&m_broadcast,
            &QUdpSocket::readyRead,
            this,
            &ControllerSynchronization::receivedDatagram);

    m_broadcast.bind(QHostAddress::AnyIPv4, 9000);
    updateViewList();
    return 0;
}

void ControllerSynchronization::setView(QObject *v)
{
    m_view = v;

    if (m_view) {
        auto *child = m_view->findChild<QObject *>("enableSync");
        connect(child, SIGNAL(checkChanged(bool)), this, SLOT(openServer(bool)));
    }
}

void ControllerSynchronization::newConnections()
{
    qDebug() << "New connection";
    m_connections << dynamic_cast<AccountSocket *>(m_server.nextPendingConnection());

    connect(m_connections.last(),
            &AccountSocket::disconnected,
            this,
            &ControllerSynchronization::onDisconnected);

    connect(m_connections.last(),
            &AccountSocket::profileChanged,
            this,
            &ControllerSynchronization::updateViewList);

    updateViewList();
}

void ControllerSynchronization::updateViewList()
{
    QVariantList vl;
    QStringList remoteList;
    m_disconnected.clear();
    for (auto *it : m_connections) {
        vl << QVariant::fromValue(it);
        remoteList << it->profile().deviceName();
    }

    for (auto it : db()->selectSyncProfile()) {
        if (!remoteList.contains(it.deviceName()) && !it.id().isNull()) {
            m_disconnected << QSharedPointer<AccountSocket>::create();
            auto s = m_disconnected.last();
            connect(s.data(),
                    &AccountSocket::profileChanged,
                    this,
                    &ControllerSynchronization::updateViewList);
            s->setLocalProfile(it);
            s->setRemoteName(it.deviceName());
            vl << QVariant::fromValue(s.data());
        }
    }

    emit connectionListChanged(vl);
}

void ControllerSynchronization::lookup()
{
    qDebug() << "Lookup" << m_server.isListening();
        qDebug() << "Broadcast"
                 << m_broadcast.writeDatagram("account_server_connect:test_server",
                                              QHostAddress(QHostAddress::Broadcast),
                                              7000);
}

void ControllerSynchronization::sync()
{
    m_client.sync();
    for (auto *it : m_connections)
        it->sync();
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
        m_client.connectToHost(addr, 9000);
}

void ControllerSynchronization::openServer(bool isOpen)
{
    if (isOpen && !m_server.isListening()) {
        m_client.close();
        m_server.listen(QHostAddress(QHostAddress::AnyIPv4), 9000);
    } else if (!isOpen) {
        for (auto *it : m_connections)
            it->close();
        m_connections.clear();
        m_server.close();
    }

    lookup();

    qDebug() << m_connections.size();
}

void ControllerSynchronization::onDisconnected()
{
    auto *send = dynamic_cast<AccountSocket *>(sender());
    m_connections.removeAll(send);

    updateViewList();
}

void ControllerSynchronization::onBeginChanged(QString id, QString begin)
{
    auto profiles = db()->selectSyncProfile();
    SynchronizationProfile profile;
    std::for_each(profiles.begin(), profiles.end(), [&profile, id](SynchronizationProfile p) {
        if (QUuid::fromString(id) == p.id())
            profile = p;
    });

    profile.setBegin(QDate::fromString(begin, "dd-MM-yyyy"));
    db()->updateSyncProfile(profile);
}
void ControllerSynchronization::onEndChanged(QString id, QString end)
{
    auto profiles = db()->selectSyncProfile();
    SynchronizationProfile profile;
    std::for_each(profiles.begin(), profiles.end(), [&profile, id](SynchronizationProfile p) {
        if (QUuid::fromString(id) == p.id())
            profile = p;
    });

    profile.setEnd(QDate::fromString(end, "dd-MM-yyyy"));
    db()->updateSyncProfile(profile);
}
