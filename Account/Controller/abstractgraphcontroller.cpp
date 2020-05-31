#include "abstractgraphcontroller.h"

int AbstractGraphController::exec()
{
    auto entries = m_db->selectEntry(currentAccount());


    return 0;
}

void AbstractGraphController::change(int way)
{
    switch (m_currentGran)
    {
    case Account::Month:
        m_currentGran = way > 0 ? Account::Year : Account::Over;
        break;
    case Account::Year:
        m_currentGran = way > 0 ? Account::Over : Account::Month;
        break;
    case Account::Over:
        m_currentGran = way > 0 ? Account::Month : Account::Year;
        break;
    }

    for(auto it: m_graphList)
        it->setGran(m_currentGran);

    increment();
}

void AbstractGraphController::increment(int)
{

}
