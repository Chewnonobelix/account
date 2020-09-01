#ifndef CONTROLLERSYNCHRONIZATION_H
#define CONTROLLERSYNCHRONIZATION_H

#include "../network/AccountSocket.h"
#include "../network/TcpServer.h"
#include "abstractcontroller.h"
#include <QHostAddress>
#include <QHostInfo>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>

class ControllerSynchronization : public AbstractController
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerSynchronization)

private:
    TcpServer m_server;
    QList<AccountSocket *> m_connections;
    QList<QSharedPointer<AccountSocket>> m_disconnected;
    QUdpSocket m_broadcast;
    AccountSocket m_client;
    QObject *m_view = nullptr;

private slots:
    void updateViewList();

protected:
public:
    ControllerSynchronization() = default;
    
    int exec();
    void setView(QObject *);
    void lookup();

signals:
    void connectionListChanged(QVariantList);

public slots:
    void newConnections();
    void receivedDatagram();
    void sync();
    void clientConnect(QHostAddress);
    void onDisconnected();
    void openServer(bool);
};


#endif // CONTROLLERSYNCHRONIZATION_H
