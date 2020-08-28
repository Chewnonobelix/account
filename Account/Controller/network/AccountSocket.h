#ifndef ACCOUNTSOCKET_HPP
#define ACCOUNTSOCKET_HPP

#include "../abstractcontroller.h"
#include "../interfacedatasave.h"
#include <QHostInfo>
#include <QTcpSocket>
#include <QtCore/qglobal.h>

class AccountSocket : public QTcpSocket
{
    Q_OBJECT

    Q_PROPERTY(QString remoteName READ remoteName NOTIFY remoteNameChanged)
    Q_PROPERTY(SynchronizationProfile localProfile READ profile NOTIFY profileChanged)

private:
    static InterfaceDataSave *m_db;
    QString m_remoteName = QString();
    SynchronizationProfile m_remoteProfile, m_localProfile;

public:
    AccountSocket();
    ~AccountSocket();

    void setSocket(QTcpSocket *);

    void parser(QString);
    bool isConnected() const;

    SynchronizationProfile profile(QString) const;
    SynchronizationProfile profile() const;

public:
    void postLocalname();
    void getRemotename();
    void postProfileid();
    void getProfileid();
    void postProfile();
    void getProfile();
    void onDisconnected();

    QString remoteName() const;

    static InterfaceDataSave *db();
    static void setDb(InterfaceDataSave *db);

public slots:
    void receiveDataSocket();
    void sync();

signals:
    void remoteNameChanged(QString);
    void profileChanged();

signals:
    void disconnected();
};

//Q_DECLARE_METATYPE(AccountSocket)

#endif // ACCOUNTSOCKET_HPP
