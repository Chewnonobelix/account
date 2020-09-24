#include "AccountSocket.h"

AccountSocket::AccountSocket() : QTcpSocket()
{
    connect(this, &QIODevice::readyRead, this, &AccountSocket::receiveDataSocket);
    connect(this, &QTcpSocket::disconnected, this, &AccountSocket::disconnected);
    connect(AbstractController::db(),
            &InterfaceDataSave::s_updateSync,
            this,
            &AccountSocket::updateLocalProfile);
}

QString AccountSocket::remoteName() const
{
    return m_remoteName;
}

AccountSocket::~AccountSocket()
{
    close();
}

void AccountSocket::receiveDataSocket()
{
    auto d = readAll();
    parser(d);
}

void AccountSocket::sync()
{
    qDebug() << "Sync";
    if (isConnected()) {
        auto create = [](QString title) -> Entry {
            Entry init;
            init.setId(QUuid::createUuid());
            init.setBlocked(true);
            init.setTitle(title);

            return init;
        };

        QMap<QString, QMap<QUuid, Entry>> entries;
        QSet<QUuid> ids;
    }
}

bool AccountSocket::isConnected() const
{
    return isOpen();
}

void AccountSocket::parser(QString data)
{
    auto split = data.split(":");
    qDebug() << "Parse" << data;
    if (split[0] != "account_api")
        return;

    if (split[1] == "post") {
        if (split[2] == "localName") {
            setRemoteName(split[3]);

            emit profileChanged();
            emit remoteNameChanged(m_remoteName);
        }
        if (split[2] == "syncId") {
            m_remoteProfile.setId(QUuid::fromString(split[3]));
        }
    }

    if (split[1] == "get") {
        if (split[2] == "localName")
            postLocalname();
        if (split[2] == "syncId")
            postProfileid();
    }
}

SynchronizationProfile AccountSocket::profile(QString remote) const
{
    auto list = AbstractController::db()->selectSyncProfile();
    SynchronizationProfile ret;

    for (auto it : list) {
        if ((it.deviceName() == remote || it.deviceName() == QHostInfo::localHostName())
            && (it.hostName() == remote || it.hostName() == QHostInfo::localHostName())) {
            ret = it;
        }
    }

    return ret;
}

SynchronizationProfile AccountSocket::profile() const
{
    return m_localProfile;
}

void AccountSocket::postLocalname()
{
    if (isConnected())
        write("account_api:post:localName:" + QHostInfo::localHostName().toLatin1());
}

void AccountSocket::getRemotename()
{
    if (isConnected())
        qDebug() << "Write" << write("account_api:get:localName");
}

void AccountSocket::postProfileid()
{
    auto id = profile(remoteName()).id();

    if (isConnected())
        write("account_api:post:syncId:" + id.toByteArray());
}

void AccountSocket::getProfileid()
{
    if (isConnected())
        write("account_api:get:syncId");
}

void AccountSocket::postProfile()
{
    if (isConnected()) {
        write("account_api:post:syncProfile" + m_remoteProfile.toString().toLatin1());
    }
}
void AccountSocket::getProfile()
{
    if (isConnected())
        write("account_api:get:syncProfile");
}

void AccountSocket::addLocalProfile()
{
    SynchronizationProfile sp;
    sp.setBegin(AbstractController::db()->selectEntry().firstKey());
    sp.setEnd(AbstractController::db()->selectEntry().lastKey());
    sp.setDeviceName(remoteName());
    sp.setHostName(QHostInfo::localHostName());
    auto id = AbstractController::db()->addSyncProfile(sp);
    sp.setId(id);
    m_localProfile = sp;
    emit profileChanged();

    updateLocalProfile();

    postProfile();
}

void AccountSocket::updateLocalProfile()
{
    if (m_localProfile.id().isNull())
        return;

    QStringList profiles = AbstractController::db()->selectProfile();

    QJsonArray localjson;
    for (auto profile : profiles) {
        AbstractController::db()->setProfile(profile);
        QStringList accounts = AbstractController::db()->selectAccount();
        for (auto account : accounts) {
            QJsonObject obj;
            QJsonArray array;
            for (auto e : AbstractController::db()->selectEntry()) {
                if (e.date() < m_localProfile.begin())
                    continue;
                if (e.date() > m_localProfile.end())
                    continue;

                array.append((QJsonObject) e);
            }
            obj.insert("entries", array);
            obj.insert("profile", profile);
            obj.insert("account", account);

            localjson.append(obj);
        }
    }

    m_localProfile.setDocument(QJsonDocument(localjson));

    QFile f(m_localProfile.id().toString() + ".json");
    f.open(QIODevice::WriteOnly);
    f.write(QJsonDocument(localjson).toJson());
    f.close();
}

void AccountSocket::removeLocalProfile()
{
    AbstractController::db()->removeSyncProfile(m_localProfile);
    m_localProfile = m_remoteProfile = SynchronizationProfile();

    emit profileChanged();
    postProfile();
}

void AccountSocket::setLocalProfile(SynchronizationProfile p)
{
    m_localProfile = p;
}

void AccountSocket::setRemoteName(QString r)
{
    m_remoteName = r;

    m_localProfile = profile(remoteName());
    updateLocalProfile();
}
