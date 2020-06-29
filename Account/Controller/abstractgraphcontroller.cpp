#include "abstractgraphcontroller.h"

AbstractGraphController::AbstractGraphController()
{
    m_graphList<<QSharedPointer<InterfaceGraph>(new TimeGraphController)
              <<QSharedPointer<InterfaceGraph>(new ControllerPieGraph);
}

AbstractGraphController::~AbstractGraphController()
{
    m_graphList.clear();
}

int AbstractGraphController::exec()
{
    increment();

    return 0;
}

void AbstractGraphController::change(int way)
{
    switch (m_currentGran)
    {
    case Account::Granularity::Month:
        m_currentGran = way > 0 ? Account::Granularity::Year : Account::Granularity::Over;
        break;
    case Account::Granularity::Year:
        m_currentGran = way > 0 ? Account::Granularity::Over : Account::Granularity::Month;
        break;
    case Account::Granularity::Over:
        m_currentGran = way > 0 ? Account::Granularity::Month : Account::Granularity::Year;
        break;
    }

    m_view->setProperty("currentGran", m_currentGran);
    for(auto it: m_graphList)
        it->setGran(m_currentGran);

    increment();
}

void AbstractGraphController::increment(int nb)
{
    switch(m_currentGran)
    {
    case Account::Granularity::Month:
        m_currentDate = m_currentDate.addMonths(nb*1);
        break;
    case Account::Granularity::Year:
        m_currentDate = m_currentDate.addYears(nb*1);
        break;
    case Account::Granularity::Over:
        break;
    }

    m_view->setProperty("currentDate", m_currentDate);
    
    for(auto it: m_graphList)
    {
        it->setDate(m_currentDate);
        it->clear();
    }

    auto e = m_db->selectEntry(currentAccount());

    bool next = true, prev = true;
    for(auto it: e)
    {
        bool b = (m_currentGran == Account::Granularity::Month && (it.date().month() == m_currentDate.month()) &&
                (it.date().year() == m_currentDate.year()));

        b |= (m_currentGran == Account::Granularity::Year && (it.date().year() == m_currentDate.year()));

        b |= (m_currentGran == Account::Granularity::Over);


        if(b)
        {
            next &= it.date() != e.uniqueKeys().last();
            prev &= it.date() != e.uniqueKeys().first();

            for(auto it2: m_graphList)
                it2->add(it);
        }
    }

    m_view->setProperty("okNext", next);
    m_view->setProperty("okPrev", prev);

    for(auto it2: m_graphList)
        it2->update();
}

void AbstractGraphController::set(const QQmlApplicationEngine & eng)
{
    m_view = eng.rootObjects().first()->findChild<QObject*>("graph")->findChild<QObject*>("root");

    connect(m_view, SIGNAL(s_zoom(int)), this, SLOT(change(int)));
    connect(m_view, SIGNAL(s_increment(int)), this, SLOT(increment(int)));

    for(auto it: m_graphList)
        it->setView(eng);
}
