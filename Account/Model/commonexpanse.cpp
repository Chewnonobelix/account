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
    if(members().size() < 2 )
        return;
    
    Total t;
    
    for(auto it: members())
        t = t + totalForMember(it);
    
    t.setValue(t.value() / members().size());
    
    QList<QPair<QString, Total>> plus, minus;
    QStringList balanced;
    for(auto it: members())
    {
        Total temp = totalForMember(it);

        if((temp - t).value() < 0)
        {
            minus<<qMakePair(it, temp - t);
        }
        else if((temp - t).value() > 0)
        {
            plus<<qMakePair(it, temp - t);
        }
        else
        {
            balanced<<it;
        }
    }
        
    auto sorter = [](QList<QPair<QString, Total>>& list) {
        for(int i = 0; i < list.size(); i++)
            for(int j = i; j < list.size(); j++)
                if(list[j].second.value() < list[i].second.value())
                {
                    auto t = list[j];
                    list[j] = list[i];
                    list[i] = t;
                }
    };


    while(!plus.isEmpty() && !minus.isEmpty())
    {
        sorter(plus); sorter(minus);
        QPair<QString, Total>& p = plus.first();  QPair<QString, Total>& m = minus.first();

        double v = std::min(p.second.value(), -m.second.value());

        p.second.setValue(p.second.value() - v);
        m.second.setValue(m.second.value() + v);

        if(p.second.value() == 0)
        {
            balanced<<p.first;
            plus.removeFirst();;
        }

        if(m.second.value() == 0)
        {
            balanced<<m.first;
            minus.removeFirst();;
        }
    }
}
