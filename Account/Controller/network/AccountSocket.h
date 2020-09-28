#ifndef ACCOUNTSOCKET_HPP
#define ACCOUNTSOCKET_HPP

#include "../core/abstractcontroller.h"
#include "../data/interfacedatasave.h"
#include "Model/synchronizationprofile.h"
#include <QDir>
#include <QHostInfo>
#include <QTcpSocket>
#include <QtCore/qglobal.h>

class AccountSocket : public QTcpSocket
{
    Q_OBJECT

    Q_PROPERTY(QString remoteName READ remoteName NOTIFY remoteNameChanged)
    Q_PROPERTY(SynchronizationProfile localProfile READ profile NOTIFY profileChanged)

private:
    QString m_remoteName = QString();
    SynchronizationProfile m_remoteProfile, m_localProfile;

    QMap<QString, QMap<QString, QString>> restapi;

public:
    AccountSocket();
    ~AccountSocket();

    void parser(QString);
    Q_INVOKABLE bool isConnected() const;

    SynchronizationProfile profile(QString) const;
    SynchronizationProfile profile() const;
    void setLocalProfile(SynchronizationProfile);
    void setRemoteName(QString);

public:
    QString remoteName() const;

    void getRemoteName();
    void postRemoteName();
    Q_INVOKABLE void onPostRemoteName(QString);
    Q_INVOKABLE void onGetRemoteName();

public slots:
    void receiveDataSocket();
    void sync();
    void addLocalProfile();
    void removeLocalProfile();

    void updateLocalProfile();
signals:
    void remoteNameChanged(QString);
    void profileChanged();

signals:
    void disconnected();
};

//Q_DECLARE_METATYPE(AccountSocket)

#endif // ACCOUNTSOCKET_HPP
