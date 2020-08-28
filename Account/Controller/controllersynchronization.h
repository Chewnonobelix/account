#ifndef CONTROLLERSYNCHRONIZATION_H
#define CONTROLLERSYNCHRONIZATION_H

#include "abstractcontroller.h"
#include "network/AccountSocket.h"
#include "network/TcpServer.h"
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
    QUdpSocket m_broadcast;
    AccountSocket m_client;
    QObject *m_view = nullptr;

    void updateViewList();

protected:
    void timerEvent(QTimerEvent *);

public:
    ControllerSynchronization() = default;
    
    int exec();
    void setView(QObject *);
    void lookup();


public slots:
    void newConnections();
    void receivedDatagram();
    void sync();
    void clientConnect(QHostAddress);
    void onDisconnected();
    void openServer(bool);
};


#endif // CONTROLLERSYNCHRONIZATION_H
