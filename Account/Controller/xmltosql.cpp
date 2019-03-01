#include "xmltosql.h"

XmltoSql::XmltoSql(ControllerXML& xml): m_xml(xml)
{

}

bool XmltoSql::transfertEntries()
{
    bool ret = true;

    auto accounts = m_xml.selectAccount();

    for(auto account: accounts)
    {
        auto entriesXml = m_xml.selectEntry(account);
        auto entriesDb = m_db.selectEntry(account);

        QMap<int, Entry> map;

        for(auto it: entriesDb)
            map[it.id()] = it;

        for(auto it: entriesXml)
        {
            if(map.contains(it.id()))
                ret &= m_db.updateEntry(it);
            else
                ret &= m_db.addEntry(it);
        }
    }

    return ret;
}

bool XmltoSql::transfertcategories()
{
    bool ret = false;

    auto categoriesXml = m_xml.selectCategory();
    auto categoriesDb = m_db.selectCategory();

    for(auto it = categoriesXml.begin(); it != categoriesXml.end(); it++)
            if(categoriesDb[it.key()] != it.value())
                m_db.addCategory(it.key(), it.value());

    return ret;

}

bool XmltoSql::exec()
{
    bool ret = true;

    if(!m_db.isConnected())
        return false;

    ret = transfertEntries();
    ret &= transfertcategories();

    return ret;
}
