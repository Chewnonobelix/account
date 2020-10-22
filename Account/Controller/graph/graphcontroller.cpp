#include "graphcontroller.h"

TimeGraphController::TimeGraphController()
    : AbstractController(), m_gran(Account::Granularity::Month)
{

}

TimeGraphController::~TimeGraphController()
{
}

int TimeGraphController::exec()
{
    return 0;
}

void TimeGraphController::setGran(Account::Granularity g)
{
    m_gran = g;
}

void TimeGraphController::add(const Entry &e)
{
    if (m_sum.isEmpty()) {
        auto ent = m_db->selectEntry();
		if (ent.isEmpty())
		 return;

		Total t;
        t.setDate(ent.first().date().addDays(-1));

        for (auto it = ent.begin(); it != ent.end() && it->date() < e.date(); it++)
            t = t + *it;
        m_sum[t.date()] = t;
    }

    QDate temp;
    switch (m_gran) {
    case Account::Granularity::Month:
        if (!m_sum.contains(e.date()))
            m_sum[e.date()] = m_sum[e.date().addDays(-1)];

        m_sum[e.date()] = m_sum[e.date()] + e;
        break;

    case Account::Granularity::Over:
    case Account::Granularity::Year:
        temp = e.date();
        temp = temp.addDays(-(e.date().day() - 1));

        if (!m_sum.contains(temp) && m_sum.count() == 1)
            m_sum[temp] = m_sum[temp.addDays(-1)];
        else if (!m_sum.contains(temp))
            m_sum[temp] = m_sum[temp.addMonths(-1)];

        m_sum[temp] = m_sum[temp] + e;
        break;
    }
}

void TimeGraphController::update()
{
    if(m_sum.isEmpty())
        return;
    
    QDate mind = (m_sum.begin()+1).key(), maxd = m_sum.lastKey();

    double min = m_sum.first().value(), max = m_sum.first().value();
    for (auto it = m_sum.begin() + 1; it != m_sum.end(); it++) {
        min = it->value() < min ? it->value() : min;
        max = it->value() > max ? it->value() : max;

        if (it.key() <= QDate::currentDate())
            emit appendMain(it.key(), it.value().value());

        if ((it.key() > QDate::currentDate())
            || (it.key() <= QDate::currentDate() && (it + 1).key() > QDate::currentDate()))
            emit appendEstimated(it.key(), it.value().value());
    }

    emit dateChanged(mind, maxd);
    emit bornValueChanged(min, max);
}

void TimeGraphController::clear()
{
    m_sum.clear();
    emit clearView();
}

void TimeGraphController::setView(const QQmlApplicationEngine & engine)
{
    auto *context = engine.rootContext();
    context->setContextProperty("_timeGraph", this);
}
