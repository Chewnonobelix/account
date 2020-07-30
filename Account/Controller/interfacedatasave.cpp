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

InterfaceDataSave::~InterfaceDataSave()
{
    QFile f("synchronization.xml");
    f.open(QIODevice::ReadWrite);    
    f.write(m_syncs.toByteArray());
    f.close();
}

void InterfaceDataSave::exec()
{
}

QList<SynchronizationProfile> InterfaceDataSave::selectSyncProfile()
{
    QDomElement root = m_syncs.documentElement();

    QList<SynchronizationProfile> ret;
    auto list = root.elementsByTagName("profile");
    for(auto i = 0; i < list.count(); i++)
    {
        QDomElement el = list.at(i).toElement();
        SynchronizationProfile sp;
        QString val = el.elementsByTagName("host").at(0).toElement().text();
        sp.setHostName(val);
        val = el.elementsByTagName("device").at(0).toElement().text();
        sp.setDeviceName(val);
        val = el.elementsByTagName("begin").at(0).toElement().text();
        sp.setbegin(QDate::fromString(val));
        val = el.elementsByTagName("end").at(0).toElement().text();
        sp.setEnd(QDate::fromString(val));
        val = el.elementsByTagName("lastSync").at(0).toElement().text();
        sp.setLastSync(QDateTime::fromString(val));
        ret<<sp;
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
        if(list.at(it).toElement().elementsByTagName("host").at(0).toElement().text() == sp.hostName() &&
                list.at(it).toElement().elementsByTagName("device").at(0).toElement().text() == sp.deviceName())
        {
           ret |= !root.removeChild(list.at(it).toElement()).isNull(); 
        }
    }
    
    return ret;
}

bool InterfaceDataSave::addSyncProfile(const SynchronizationProfile& sp)
{
    QDomElement root = m_syncs.documentElement();
    auto list = root.elementsByTagName("profile");
    
    for(auto it = 0; it < list.size(); it++)
    {
        if(list.at(it).toElement().elementsByTagName("host").at(0).toElement().text() == sp.hostName() &&
                list.at(it).toElement().elementsByTagName("device").at(0).toElement().text() == sp.deviceName())
            return false;
    }
    
    auto el = m_syncs.createElement("profile");
    root.appendChild(el);
    
    auto func = [&root, &el](QDomDocument doc, QString tag, QString val) {
        auto el1 = doc.createElement(tag);
        auto text = doc.createTextNode(val);
        el1.appendChild(text);
        el.appendChild(el1);        
    };
    
    func(m_syncs, "host", sp.hostName());
    func(m_syncs, "device", sp.deviceName());
    func(m_syncs, "begin", sp.begin().toString());
    func(m_syncs, "end", sp.end().toString());
    func(m_syncs, "lastSync", sp.lastSync().toString());
    
    return true;
}

bool InterfaceDataSave::updateSyncProfile(const SynchronizationProfile& sp)
{
    QDomElement root = m_syncs.documentElement();
    auto list = root.elementsByTagName("profile");
    bool ret = false;
    for(auto it = 0; it < list.size(); it++)
    {
        if(list.at(it).toElement().elementsByTagName("host").at(0).toElement().text() == sp.hostName() &&
                list.at(it).toElement().elementsByTagName("device").at(0).toElement().text() == sp.deviceName())
        {
           ret = true;
           QDomElement el = list.at(it).toElement();
           el.elementsByTagName("begin").at(0).firstChild().setNodeValue(sp.begin().toString());
           el.elementsByTagName("end").at(0).firstChild().setNodeValue(sp.end().toString());
           el.elementsByTagName("lastSync").at(0).firstChild().setNodeValue(sp.lastSync().toString());
           
        }
    }
    
    return ret;
}
