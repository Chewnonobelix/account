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

    Q_PROPERTY(QString remoteName READ remoteName)

private:
    QTcpSocket* m_socket = nullptr;
    QString m_remoteName = QString();
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

    QString remoteName() const;

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
    QObject *m_view = nullptr;

protected:
    void timerEvent(QTimerEvent *);

public:
    ControllerSynchronization() = default;
    
    int exec();
    void setView(QObject *);
    void lookup();

    void openServer(bool);

public slots:
    void newConnections();
    void receivedDatagram();
    void sync();
    void clientConnect(QHostAddress);
};

Q_DECLARE_METATYPE(AccountSocket)

#endif // CONTROLLERSYNCHRONIZATION_H
