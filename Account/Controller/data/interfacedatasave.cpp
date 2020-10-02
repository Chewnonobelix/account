#include "interfacedatasave.h"

InterfaceDataSave::InterfaceDataSave()
{
    QFile f("synchronization.xml");
    f.open(QIODevice::ReadWrite);    
    m_syncs.setContent(&f);
    f.close();
    
    if(m_syncs.toByteArray().isEmpty())
        m_syncs.setContent(QString("<sync_profile />"));
}

InterfaceDataSave::InterfaceDataSave(const InterfaceDataSave &i)
    : QObject(nullptr), backup(i.backup), m_currentProfile(i.currentProfile()),
      m_currentAccount(i.currentAccount()), m_path(i.m_path)
{
    QFile f("synchronization.xml");
    f.open(QIODevice::ReadWrite);
    m_syncs.setContent(&f);
    f.close();

    if (m_syncs.toByteArray().isEmpty())
        m_syncs.setContent(QString("<sync_profile />"));
}

InterfaceDataSave::~InterfaceDataSave()
{
    QFile f("synchronization.xml");
    f.open(QIODevice::WriteOnly);
    f.write(m_syncs.toByteArray());
    f.close();
}

void InterfaceDataSave::exec()
{
}

QMap<QUuid, SynchronizationProfile> InterfaceDataSave::selectSyncProfile()
{
    QDomElement root = m_syncs.documentElement();

    QMap<QUuid, SynchronizationProfile> ret;
    auto list = root.elementsByTagName("profile");
    for(auto i = 0; i < list.count(); i++)
    {
        QDomElement el = list.at(i).toElement();
        SynchronizationProfile sp;
        QString val = el.elementsByTagName("host").at(0).toElement().text();
        sp.setHostName(val);
        val = el.elementsByTagName("device").at(0).toElement().text();
        sp.setDeviceName(val);
        val = el.elementsByTagName("start").at(0).toElement().text();
        sp.setBegin(QDate::fromString(val));
        if (!sp.begin().isValid())
            sp.setBegin(QDate::currentDate());
        val = el.elementsByTagName("end").at(0).toElement().text();
        sp.setEnd(QDate::fromString(val));
        if (!sp.end().isValid())
            sp.setEnd(QDate::currentDate());
        val = el.elementsByTagName("lastSync").at(0).toElement().text();
        sp.setLastSync(QDateTime::fromString(val));

        val = el.elementsByTagName("id").at(0).toElement().text();
        sp.setId(QUuid::fromString(val));
        ret[sp.id()] = sp;
    }
    return ret;
}

bool InterfaceDataSave::removeSyncProfile(const SynchronizationProfile& sp)
{
    QDomElement root = m_syncs.documentElement();
    auto list = root.elementsByTagName("profile");
    bool ret = false;
    for(auto it = 0; it < list.size(); it++)
    {
        if (list.at(it).toElement().elementsByTagName("id").at(0).toElement().text()
            == sp.id().toString()) {
            ret |= !root.removeChild(list.at(it).toElement()).isNull();
            emit s_updateSync();
        }
    }
    
    return ret;
}

QUuid InterfaceDataSave::addSyncProfile(const SynchronizationProfile &sp)
{
    QDomElement root = m_syncs.documentElement();
    auto list = root.elementsByTagName("profile");
    
    for(auto it = 0; it < list.size(); it++)
    {
        if(list.at(it).toElement().elementsByTagName("host").at(0).toElement().text() == sp.hostName() &&
                list.at(it).toElement().elementsByTagName("device").at(0).toElement().text() == sp.deviceName())
            return QUuid();
    }
    
    auto el = m_syncs.createElement("profile");
    root.appendChild(el);

    auto func = [&root, &el](QDomDocument doc, QString tag, QString val) {
        auto el1 = doc.createElement(tag);
        auto text = doc.createTextNode(val);
        el1.appendChild(text);
        el.appendChild(el1);
    };

    auto id = QUuid::createUuid();

    func(m_syncs, "host", sp.hostName());
    func(m_syncs, "device", sp.deviceName());
    func(m_syncs, "start", sp.begin().toString());
    func(m_syncs, "end", sp.end().toString());
    func(m_syncs, "lastSync", QDateTime::currentDateTime().toString());
    func(m_syncs, "id", id.toString());

    emit s_updateSync();
    return id;
}

bool InterfaceDataSave::updateSyncProfile(const SynchronizationProfile& sp)
{
    QDomElement root = m_syncs.documentElement();
    auto list = root.elementsByTagName("profile");
    bool ret = false;
    for(auto it = 0; it < list.size(); it++)
    {
        if (list.at(it).toElement().elementsByTagName("id").at(0).toElement().text()
            == sp.id().toString()) {
            ret = true;
            QDomElement el = list.at(it).toElement();

            el.elementsByTagName("start").at(0).firstChild().setNodeValue(sp.begin().toString());
            el.elementsByTagName("end").at(0).firstChild().setNodeValue(sp.end().toString());
            el.elementsByTagName("lastSync")
                .at(0)
                .firstChild()
                .setNodeValue(sp.lastSync().toString());

            emit s_updateSync();
        }
    }
    
    return ret;
}

void InterfaceDataSave::setProfile(QString profile)
{
    m_currentProfile = profile;
}

QString InterfaceDataSave::currentProfile() const
{
    return m_currentProfile;
}

void InterfaceDataSave::setCurrentAccount(QString account)
{
    m_currentAccount = account;
}

QString InterfaceDataSave::currentAccount() const
{
    return m_currentAccount;
}
