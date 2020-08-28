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
    qDebug() << "New connection";
    m_connections << dynamic_cast<AccountSocket *>(m_server.nextPendingConnection());

    connect(m_connections.last(),
            &AccountSocket::disconnected,
            this,
            &ControllerSynchronization::onDisconnected);

    updateViewList();
}

void ControllerSynchronization::updateViewList()
{
    auto *list = m_view->findChild<QObject *>("syncProfiles");
    QVariantList vl;

    for (auto *it : m_connections)
        vl << QVariant::fromValue(it);

    list->setProperty("model", vl);
}

void ControllerSynchronization::timerEvent(QTimerEvent *)
{
    //    if (m_server.isListening())
    //        lookup();
}

void ControllerSynchronization::lookup()
{
    qDebug() << "Lookup" << m_server.isListening();
    if (m_server.isListening())
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
        lookup();
    } else if (!isOpen) {
        for (auto *it : m_connections)
            it->close();
        m_connections.clear();
        m_server.close();
    }

    qDebug() << m_connections.size();
}

void ControllerSynchronization::onDisconnected()
{
    auto *send = dynamic_cast<AccountSocket *>(sender());
    m_connections.removeAll(send);

    updateViewList();
}

