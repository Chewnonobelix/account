#include "commonexpanse.h"
#include <QJsonArray>
CommonExpanse::CommonExpanse()
{
    setId(QUuid());
    setBegin(QDate::currentDate());
    setTitle(QString());
    setIsClose(false);
}

CommonExpanse::CommonExpanse(const QJsonObject &obj) : MetaData(obj)
{
 auto keys = obj.keys();
 keys.removeAll("id");
 keys.removeAll("title");
 keys.removeAll("isClose");
 keys.removeAll("begin");

 for (auto it : keys) {
  auto array = obj[it].toArray();
  for (auto it2 : array) {
   Entry e(it2.toObject());
   addEntries(it, e);
  }
 }
}

CommonExpanse::operator QJsonObject() const
{
 auto ret = MetaData::operator QJsonObject();

 for (auto it : members()) {
  QJsonArray array;
  auto list = entries().values(it);
  for (auto it2 : list)
   array << QJsonObject(it2);
  ret[it] = array;
 }
 return ret;
}

QUuid CommonExpanse::id() const
{
    return metaData<QUuid>("id");
}

void CommonExpanse::setId(QUuid id)
{
    setMetadata("id", id);
}

QDate CommonExpanse::begin() const
{
    return metaData<QDate>("begin");
}

void CommonExpanse::setBegin(QDate begin)
{
    setMetadata("begin", begin);
}

QString CommonExpanse::title() const
{
    return metaData<QString>("title");
}

void CommonExpanse::setTitle(QString title)
{
    setMetadata("title", title);
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
    return metaData<bool>("isClose");
}

void CommonExpanse::setIsClose(bool isClose)
{
    setMetadata("isClose", isClose);
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
    if(!m_entries.contains(name, ee))
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
    
    m_closing.clear();
    
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
        
        Closing c;
        c.from = m.first;
        c.to = p.first;
        c.value = v;
        
        m_closing<<c;
        
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

QList<QVariant> CommonExpanse::closing() const
{
    QVariantList ret;

    for(auto it: m_closing)
        ret<<QVariant::fromValue(it);
    
    return ret;
}

bool CommonExpanse::removeEntry(QString member, Entry e)
{
    return m_entries.remove(member, e) > 0;
}
