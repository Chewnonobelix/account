#include "commonexpanse.h"

int CommonExpanse::id() const
{
    return m_id;
}

void CommonExpanse::setId(int id)
{
    m_id = id;
}

QDate CommonExpanse::begin() const
{
    return m_begin;
}

void CommonExpanse::setBegin(QDate begin)
{
    m_begin = begin;
}

QString CommonExpanse::title() const
{
    return m_title;
}

void CommonExpanse::setTitle(QString title)
{
    m_title = title;
}

QMultiMap<QString, Entry> CommonExpanse::entries() const
{
    return m_entries;
}

void CommonExpanse::setEntries(QMultiMap<QString, Entry> entries)
{
    m_entries = entries;
}

bool CommonExpanse::isClose() const
{
    return m_isClose;
}

void CommonExpanse::setIsClose(bool isClose)
{
    m_isClose = isClose;
}

QStringList CommonExpanse::members() const
{
    return m_entries.uniqueKeys();
}

QVariantList CommonExpanse::entries(QString name) const
{
    QVariantList list;

    for(auto it: m_entries.values(name))
        list<<QVariant::fromValue(it);

    return list;
}

void CommonExpanse::addEntries(QString name, Entry ee)
{
    m_entries.insert(name, ee);
}

Total CommonExpanse::totalForMember(QString member) const
{
    Total ret;
    
    auto el = m_entries.values(member);
    
    for(auto it: el)
        ret = ret + it;
    
    return ret;
}

QVariantMap CommonExpanse::totalForMembers() const
{
    QVariantMap ret; 
    auto list = members();
    for(auto it: list)
    {
        ret[it] = QVariant::fromValue(totalForMember(it));
    }
    
    return ret;    
}

void CommonExpanse::equilibrate()
{
    Total t;
    
    for(auto it: members())
        t = t + totalForMember(it);
    
    t.setValue(t.value() / members().size());
    
    qDebug()<<"Median"<<t.value();
}
