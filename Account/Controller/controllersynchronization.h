#ifndef CONTROLLERSYNCHRONIZATION_H
#define CONTROLLERSYNCHRONIZATION_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QHostInfo>
#include <QUdpSocket>
#include "abstractcontroller.h"

class AccountSocket: public AbstractController
{
    Q_OBJECT
private:
    QTcpSocket* m_socket = nullptr;
    
    
public:
    AccountSocket() = default;
    AccountSocket(const AccountSocket&);
    ~AccountSocket();
    AccountSocket& operator =(const AccountSocket&);
    
    void setSocket(QTcpSocket*);
    
    int exec();
    
public slots:
    void receiveDataSocket();
    void sync();
    void connectTo(QHostAddress);
};

class ControllerSynchronization: public AbstractController
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerSynchronization)
    
    
private:
    QTcpServer m_server;
    QList<AccountSocket> m_connections;
    QUdpSocket m_broadcast;
    AccountSocket m_client;

public:
    ControllerSynchronization() = default;
    
    int exec();

    Q_INVOKABLE void lookup();

public slots:
    void newConnections();
    void receivedDatagram();
    void sync();
    void clientConnect(QHostAddress);
};

#endif // CONTROLLERSYNCHRONIZATION_H
