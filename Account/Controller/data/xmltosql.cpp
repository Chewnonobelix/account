#include "xmltosql.h"

TransfertDatabase::TransfertDatabase()
{

}

bool TransfertDatabase::transfertEntries()
{
    bool ret = true;

    for (auto it : accounts) {
        m_db->setCurrentAccount(it);
        for (auto it2 : m_db->selectEntry()) {
            ret &= m_backup->addEntry(it2);
            ret &= m_backup->updateEntry(it2);
        }
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

void TransfertDatabase::run()
{
    m_sucess = true;
    accounts = m_db->selectAccount();

    m_sucess &= transfertEntries();
    m_sucess &= transfertCategories();
    m_sucess &= transfertBudget();
    m_sucess &= transfertFrequency();
    m_sucess &= transfertCommon();
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
