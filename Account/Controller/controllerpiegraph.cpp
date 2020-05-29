#include "controllerpiegraph.h"

ControllerPieGraph::ControllerPieGraph(QObject*)
{
    
}

int ControllerPieGraph::exec()
{
    increment();
    return 0;
}

void ControllerPieGraph::init(const QQmlApplicationEngine &engine)
{
    connect(m_db, InterfaceDataSave::s_updateEntry, this, ControllerPieGraph::exec);
    m_income = engine.rootObjects().first()->findChild<QObject*>("incomingPie");
    m_outcome = engine.rootObjects().first()->findChild<QObject*>("outcomePie");
    m_view = engine.rootObjects().first()->findChild<QObject*>("pieCategory");

    connect(m_view, SIGNAL(s_zoom(int)), this, SLOT(change(int)));
}

Account::Granularity next(Account::Granularity g, int way)
{
    switch (g)
    {
    case Account::Month:
        return way > 0 ? Account::Year : Account::Over;
        break;
    case Account::Year:
        return way > 0 ? Account::Over : Account::Month;
        break;
    case Account::Over:
        return way > 0 ? Account::Month : Account::Year;
        break;
    }
}

void ControllerPieGraph::change(int nGranularity)
{
    m_gran = next(m_gran, nGranularity);

    m_view->setProperty("gran", m_gran);
    increment();
}

bool isValid(Account::Granularity g, QDate d, QDate c)
{
    bool ret = false;
    switch(g)
    {
    case Account::Month:
        ret = (d.month() == c.month() && d.year() == c.year());
        break;
    case Account::Year:
        ret = d.year() == c.year();
        break;
    case Account::Over:
        ret = true;
        break;
    }


    return ret;
}

void ControllerPieGraph::increment(int inc)
{
    auto entries = m_db->selectEntry(currentAccount());

    QDate date = m_view->property("currentDate").toDate();

    QMap<QString, QMap<QString, Total>> sum;
    for(auto it: entries)
    {
        if(isValid(m_gran, it.date(), date))
        {
            Total t = sum[it.type()][it.info().category()];
            t = t + it;
            sum[it.type()][it.info().category()] = t;
        }
    }

    QMetaObject::invokeMethod(m_income, "clear");
    QMetaObject::invokeMethod(m_outcome, "clear");

    auto setter = [](QMap<QString, Total> s, QObject* pie) {
        for(auto it = s.begin(); it != s.end(); it++)
        {
            QMetaObject::invokeMethod(pie, "append", Q_ARG(QVariant, it.key().isEmpty() ? tr("Unknow") : it.key()), Q_ARG(QVariant, it.value().value()));
        }
    };

    setter(sum["income"], m_income);
    setter(sum["outcome"], m_outcome);

}
