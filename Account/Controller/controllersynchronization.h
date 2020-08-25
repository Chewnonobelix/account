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
    QString remoteName = QString();
    SynchronizationProfile remoteProfile;

public:
    AccountSocket() = default;
    AccountSocket(const AccountSocket&);
    ~AccountSocket();
    AccountSocket &operator=(const AccountSocket &);

    void setSocket(QTcpSocket *);

    int exec();

    void parser(QString);
    bool isConnected() const;

    SynchronizationProfile profile(QString) const;

public:
    void postLocalname();
    void getRemotename();
    void postProfileid();
    void getProfileid();
    void postProfile();
    void getProfile();

public slots:
    void receiveDataSocket();
    void sync();
    void connectTo(QHostAddress);
    void close();
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

protected:
    void timerEvent(QTimerEvent *);

public:
    ControllerSynchronization() = default;
    
    int exec();

    void lookup();

    void openServer(bool);

public slots:
    void newConnections();
    void receivedDatagram();
    void sync();
    void clientConnect(QHostAddress);
};

#endif // CONTROLLERSYNCHRONIZATION_H
