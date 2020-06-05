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

void AbstractGraphController::increment(int nb)
{
    switch(m_currentGran)
    {
    case Account::Month:
        m_currentDate = m_currentDate.addMonths(nb*1);
        break;
    case Account::Year:
        m_currentDate = m_currentDate.addYears(nb*1);
        break;
    case Account::Over:
        break;
    }

    m_view->setProperty("currentDate", m_currentDate);
    
    for(auto it: m_graphList)
    {
        it->setDate(m_currentDate);
        it->clear();
    }

    auto e = m_db->selectEntry(currentAccount());

    for(auto it: e)
    {
        bool b = (m_currentGran == Account::Month && (it.date().month() == m_currentDate.month()) &&
                (it.date().year() == m_currentDate.year()));

        b |= (m_currentGran == Account::Year && (it.date().year() == m_currentDate.year()));

        b |= (m_currentGran == Account::Over);

        if(b)
        {
            for(auto it2: m_graphList)
                it2->add(it);
        }
    }

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
