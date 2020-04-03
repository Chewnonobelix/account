#include "xmltosql.h"

TransfertDatabase::TransfertDatabase(InterfaceDataSave* backup): m_backup(backup)
{

}

bool TransfertDatabase::transfertEntries()
{
    bool ret = true;

    for(auto it: accounts)
        for(auto it2: m_db->selectEntry(it))
        {
            ret &= m_backup->addEntry(it2);
            ret &= m_backup->updateEntry(it2);
        }

    return ret;
}

bool TransfertDatabase::transfertCategories()
{
    bool ret = true;

    for(auto it: accounts)
    {
        m_db->setCurrentAccount(it);
        m_backup->setCurrentAccount(it);

        auto cats = m_db->selectCategory();
        for(auto it2 = cats.begin(); it2 != cats.end(); it2++)
        {
            m_backup->addCategory(it2.value(), it2.key());
        }
    }
    return ret;
}

int TransfertDatabase::exec()
{
    bool ret = true;
    accounts = m_db->selectAccount();

    ret &= transfertEntries();
    ret &= transfertCategories();
    ret &= transfertBudget();
    ret &= transfertFrequency();
    ret &= transfertCommon();

    return ret;
}

bool TransfertDatabase::transfertBudget()
{
    bool ret = true;

    for(auto it: accounts)
    {
        m_db->setCurrentAccount(it);
        m_backup->setCurrentAccount(it);

        for(auto it2: m_db->selectBudgets())
        {
            ret &= m_backup->addBudget(it2);
            ret &= m_backup->updateBudget(it2);
        }
    }
    return ret;
}

bool TransfertDatabase::transfertFrequency()
{
    bool ret = true;

    for(auto it: accounts)
    {
        m_db->setCurrentAccount(it);
        m_backup->setCurrentAccount(it);

        for(auto it2: m_db->selectFrequency())
        {
            ret &= m_backup->addFrequency(it2);
            ret &= m_backup->updateFrequency(it2);
        }
    }
    return ret;
}

bool TransfertDatabase::transfertCommon()
{
    bool ret = true;

    for(auto it: accounts)
    {
        m_db->setCurrentAccount(it);
        m_backup->setCurrentAccount(it);

        for(auto it2: m_db->selectCommon())
        {
            ret &= m_backup->addCommon(it2);
            ret &= m_backup->updateCommon(it2);
        }
    }
    return ret;
}
