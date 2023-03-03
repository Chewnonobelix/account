#include "xmltosql.h"

TransfertDatabase::TransfertDatabase()
{

}

template<class S, class U, class A>
bool transfert(S select, U update, A add)
{
    bool ret = true;

//    for (auto it2 : select()) {
//        ret &= add(it2);
//        ret &= update(it2);
//    }
    return ret;
}

bool TransfertDatabase::transfertEntries()
{
    bool ret = true;

    auto s = [this]() {
        return QMap<QUuid, Entry>();
//        return m_db->selectEntry();
    };

    auto a = [this](auto e) {
        return m_db->addEntry(e);
    };

    auto u = [this](auto e) {
        return m_db->updateEntry(e);
    };

    for (auto it : accounts) {
        m_db->setCurrentAccount(it);
        m_backup->setCurrentAccount(it);
        ret &= transfert(s, u, a);
    }
    return ret;
}

bool TransfertDatabase::transfertCategories()
{
    bool ret = true;

    auto s = [this]() {
        QMap<QUuid, Category> ret;
        ret = m_db->selectCategory()[Account::TypeEnum::Income];
        for(auto c: m_db->selectCategory()[Account::TypeEnum::Outcome])
            ret[c.id()] = c;

        return ret;
    };

    auto a = [this](auto e) {
        return m_db->addCategory(e);
    };

    auto u = [this](auto e) {
        return m_db->updateCategory(e);
    };

    for (auto it : accounts) {
        m_db->setCurrentAccount(it);
        m_backup->setCurrentAccount(it);
        ret &= transfert(s, u, a);
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
    m_sucess &= transfertDebt();
}

bool TransfertDatabase::transfertBudget()
{
    bool ret = true;

    auto s = [this]() {
        return m_db->selectBudgets();
    };

    auto a = [this](auto e) {
        return m_db->addBudget(e);
    };

    auto u = [this](auto e) {
        return m_db->updateBudget(e);
    };

    for (auto it : accounts) {
        m_db->setCurrentAccount(it);
        m_backup->setCurrentAccount(it);
        ret &= transfert(s, u, a);
    }
    return ret;
}

bool TransfertDatabase::transfertFrequency()
{
    bool ret = true;

    auto s = [this]() {
        return m_db->selectFrequency();
    };

    auto a = [this](auto e) {
        return m_db->addFrequency(e);
    };

    auto u = [this](auto e) {
        return m_db->updateFrequency(e);
    };

    for (auto it : accounts) {
        m_db->setCurrentAccount(it);
        m_backup->setCurrentAccount(it);
        ret &= transfert(s, u, a);
    }
    return ret;
}

bool TransfertDatabase::transfertCommon()
{
    bool ret = true;

    auto s = [this]() {
        return m_db->selectCommon();
    };

    auto a = [this](auto e) {
        return m_db->addCommon(e);
    };

    auto u = [this](auto e) {
        return m_db->updateCommon(e);
    };

    for (auto it : accounts) {
        m_db->setCurrentAccount(it);
        m_backup->setCurrentAccount(it);
        ret &= transfert(s, u, a);
    }
    return ret;
}

bool TransfertDatabase::transfertDebt()
{
    bool ret = true;

    auto s = [this]() {
        return m_db->selectDebt();
    };

    auto a = [this](auto e) {
        return m_db->addDebt(e);
    };

    auto u = [this](auto e) {
        return m_db->updateDebt(e);
    };

    for (auto it : accounts) {
        m_db->setCurrentAccount(it);
        m_backup->setCurrentAccount(it);
        ret &= transfert(s, u, a);
    }
    return ret;
}
