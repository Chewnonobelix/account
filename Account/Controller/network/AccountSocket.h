#ifndef ACCOUNTSOCKET_HPP
#define ACCOUNTSOCKET_HPP

#include "../core/abstractcontroller.h"
#include "../data/interfacedatasave.h"
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
    Q_INVOKABLE bool isConnected() const;

    SynchronizationProfile profile(QString) const;
    SynchronizationProfile profile() const;
    void setLocalProfile(SynchronizationProfile);
    void setRemoteName(QString);

public:
    Q_INVOKABLE void postLocalname();
    Q_INVOKABLE void getRemotename();
    Q_INVOKABLE void postProfileid();
    Q_INVOKABLE void getProfileid();
    Q_INVOKABLE void postProfile();
    Q_INVOKABLE void getProfile();

    QString remoteName() const;

    static InterfaceDataSave *db();
    static void setDb(InterfaceDataSave *db);

public slots:
    void receiveDataSocket();
    void sync();
    void addLocalProfile();
    void removeLocalProfile();

signals:
    void remoteNameChanged(QString);
    void profileChanged();

signals:
    void disconnected();
};

//Q_DECLARE_METATYPE(AccountSocket)

#endif // ACCOUNTSOCKET_HPP
