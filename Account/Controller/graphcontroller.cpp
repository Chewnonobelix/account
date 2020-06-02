#include "graphcontroller.h"

TimeGraphController::TimeGraphController(): AbstractController(), m_view(nullptr), m_gran(Account::Month)
{

}

TimeGraphController::~TimeGraphController()
{
}


void TimeGraphController::set(QObject * view)
{
    m_view = view;
    connect(m_db, InterfaceDataSave::s_updateEntry, this, TimeGraphController::exec);
    
    if(m_view)
    {
        connect(m_view, SIGNAL(s_increment(int)), this, SLOT(increment(int)));
        connect(m_view, SIGNAL(s_zoom(int)), this, SLOT(change(int)));
    }
}

QMap<QDate, Total> TimeGraphController::sum() const
{
    return m_sum;
}

int TimeGraphController::exec()
{
//    m_sum.clear();

//    if(!m_view)
//        return -1;

//    QMetaObject::invokeMethod(m_view, "clear");
//    auto model = m_db->selectEntry(currentAccount());
//    auto dateList = model.uniqueKeys();
//    std::sort(dateList.begin(), dateList.end());
//    QList<QDate> keysT;
//    QDate minDate , maxDate;

//    if(!dateList.isEmpty())
//    {
//        minDate = dateList.first();
//        maxDate = dateList.last();
//    }

//    QDate cd = minDate;
//    keysT<<minDate;
//    while(cd != maxDate)
//    {
//        cd = cd.addDays(1);
//        keysT<<cd;
//    }

//    Total t;

//    QList<QPair<Total, bool>> sum;
//    for(auto it: keysT)
//    {
//        auto l = model.values(it);
//        if(l.isEmpty())
//        {
//            Entry e;
//            e.setDate(it);
//            e.setValue(0);
//            Information i;
//            i.setEstimated(it > QDate::currentDate());
//            e.setInfo(i);
//            l<<e;
//        }


//        for(auto it2: l)
//        {
//            t = t + it2;
//        }

//        sum.append(qMakePair(t, l.first().info().estimated()));

//        m_sum[t.date()] = t;
//    }

//    increment();

//    emit s_sum();
    return 0;
}


void TimeGraphController::setDate(QDate d)
{
    m_view->setProperty("currentDate", d);
}

void TimeGraphController::setGran(Account::Granularity g)
{
    m_view->setProperty("currentGran", g);
    m_gran = g;

    if(m_gran == Account::Year) m_view->setProperty("granularity", tr("one year"));
    if(m_gran == Account::Month) m_view->setProperty("granularity", tr("one month"));
    if(m_gran == Account::Over) m_view->setProperty("granularity", tr("all years"));
}

void TimeGraphController::increment(int inc)
{

//    QMap<QDate, Total> ret;
//    QDate date = m_view->property("currentDate").toDate();
//    int cMonth = date.month();
//    int cYear = date.year();
//    QDate itDate, lastDay;
//    QSharedPointer<int> minVal, maxVal;

//    auto setMin = [&](double val) {
//        if(minVal.isNull())
//        {
//            minVal.reset(new int);
//            *minVal = val;
//        }
//        else if(val < *minVal)
//        {
//            *minVal = val;
//        }
//    };

//    auto setMax = [&](double val) {
//        if(minVal.isNull())
//        {
//            maxVal.reset(new int);
//            *maxVal = val;
//        }
//        else if(val > *maxVal)
//        {
//            *maxVal = val;
//        }
//    };

//    m_view->setProperty("okPrev", (m_gran != Account::Over));
//    m_view->setProperty("okNext", (m_gran != Account::Over));

//    switch(m_gran)
//    {
//    case Account::Month:
//        cMonth += inc;

//        if(cMonth == 0)
//        {
//            cMonth = 12;
//            cYear --;
//        }
//        else if(cMonth == 13)
//        {
//            cMonth = 1;
//            cYear ++;
//        }


//        for(auto it: m_sum)
//        {
//            if(it.date().month() == cMonth && it.date().year() == cYear)
//            {
//                ret[it.date()] = it;
//                setMax(it.value());
//                setMin(it.value());
//            }
//        }
//        break;

//    case Account::Year:
//        cYear += inc;
//        itDate.setDate(cYear, 1, 1);
//        if(m_sum.contains(itDate))
//            ret[itDate] = m_sum[itDate];
//        itDate = itDate.addDays(30);

//        if(!m_sum.contains(itDate.addMonths(-1)))
//            ret[m_sum.first().date()] = m_sum.first();
//        for(int i = 0; i < 12; i++)
//        {
//            if(m_sum.contains(itDate))
//            {
//                ret[itDate] = m_sum[itDate];
//                setMax(m_sum[itDate].value());
//                setMin(m_sum[itDate].value());
//            }
//            else
//            {
//                QDate wtf;
//                wtf.setDate(itDate.year(), itDate.month()-1, 1);
//                if(m_sum.contains(wtf))
//                    ret[m_sum.last().date()] = m_sum.last();
//            }
//            itDate = itDate.addMonths(1);
//            itDate = itDate.addDays(itDate.daysInMonth() - itDate.day());
//        }
//        break;

//    case Account::Over:
//        itDate = m_sum.first().date();
//        ret[itDate] = m_sum[itDate];
//        itDate = itDate.addDays(itDate.daysInMonth() - itDate.day());
//        lastDay = m_sum.last().date();
//        while(itDate < lastDay)
//        {
//            if(m_sum.contains(itDate))
//            {
//                ret[itDate] = m_sum[itDate];
//                setMax(m_sum[itDate].value());
//                setMin(m_sum[itDate].value());
//            }

//            itDate = itDate.addMonths(1);
//            itDate = itDate.addDays(itDate.daysInMonth() - itDate.day());

//        }

//        if(!ret.contains(lastDay))
//        {
//            ret[lastDay] = m_sum[lastDay];
//            setMax(m_sum[lastDay].value());
//            setMin(m_sum[lastDay].value());
//        }
//        break;
//    }
    
//    date.setDate(cYear, cMonth, 1);

//    m_view->setProperty("currentDate", date);

//    QDate minDate, maxDate;
//    if(!ret.isEmpty())
//    {
//        minDate = ret.first().date();
//        maxDate = ret.last().date();
//    }

//    if(!ret.isEmpty() && !m_sum.isEmpty())
//    {
//        m_view->setProperty("okPrev", !(ret.first() == m_sum.first()));
//        m_view->setProperty("okNext", !(ret.last() == m_sum.last()));
//    }

//    if(!maxVal.isNull())
//    {
//        *maxVal = *maxVal > 0 ? *maxVal * 1.05 : *maxVal * .95;
//        *minVal = *minVal < 0 ? *minVal * 1.05 : *minVal * .95;


//        QMetaObject::invokeMethod(m_view, "setMinMaxDate", Q_ARG(QVariant, minDate), Q_ARG(QVariant, maxDate));
//        QMetaObject::invokeMethod(m_view, "setMinMaxValue", Q_ARG(QVariant, *minVal), Q_ARG(QVariant, *maxVal));
//    }

//    for(auto it = ret.begin(); it != ret.end(); it++)
//    {
//        if(it.value().date() > QDate::currentDate() && m_view->property("estimatedCount").toInt() == 0 && it != ret.begin())
//            QMetaObject::invokeMethod(m_view, "addDataEstimated", Q_ARG(QVariant,(it-1).value().date()), Q_ARG(QVariant, (it-1).value().value()));
//        if(it.value().date() > QDate::currentDate())
//            QMetaObject::invokeMethod(m_view, "addDataEstimated", Q_ARG(QVariant,it.value().date()), Q_ARG(QVariant, it.value().value()));
//        else
//            QMetaObject::invokeMethod(m_view, "addDataMain", Q_ARG(QVariant,it.value().date()), Q_ARG(QVariant, it.value().value()));

//    }
}

void TimeGraphController::add(const Entry & e)
{
    QMetaObject::invokeMethod(m_view, "clear");

    if(m_sum.isEmpty())
    {
        auto ent = m_db->selectEntry(currentAccount());
        Total t;
        for(auto it = ent.begin(); it != ent.end() && it->date() < e.date(); it++)
            t = t + *it;
        m_sum[t.date()] = t;
    }
    
    QDate temp;
    switch(m_gran)
    {
    case Account::Month:
        if(!m_sum.contains(e.date()))
            m_sum[e.date()] = m_sum[e.date().addDays(-1)];

        m_sum[e.date()] = m_sum[e.date()] +  e;
        break;

    case Account::Over:
    case Account::Year:
        temp = e.date();
        temp = temp.addDays(-(e.date().day()-1));
        
        if(!m_sum.contains(temp) && m_sum.count() == 1)
            m_sum[temp] = m_sum[temp.addDays(-1)];
        else if(!m_sum.contains(temp))
            m_sum[temp] = m_sum[temp.addMonths(-1)];            
        
        m_sum[temp] = m_sum[temp] + e;
        break;
    }

    QDate mind = m_sum.firstKey(), maxd = m_sum.lastKey();
    
    double min = m_sum.first().value(), max = m_sum.first().value();
    for(auto it = m_sum.begin(); it != m_sum.end(); it++)
    {
        min = it->value() < min ? it->value() : min;
        max = it->value() > max ? it->value() : max;

        if(it.value().date() > QDate::currentDate() && m_view->property("estimatedCount").toInt() == 0 && it != m_sum.begin())
            QMetaObject::invokeMethod(m_view, "addDataEstimated", Q_ARG(QVariant,(it-1).value().date()), Q_ARG(QVariant, (it-1).value().value()));
        if(it.value().date() > QDate::currentDate())
            QMetaObject::invokeMethod(m_view, "addDataEstimated", Q_ARG(QVariant,it.value().date()), Q_ARG(QVariant, it.value().value()));
        else
            QMetaObject::invokeMethod(m_view, "addDataMain", Q_ARG(QVariant,it.value().date()), Q_ARG(QVariant, it.value().value()));

    }
    
    
    QMetaObject::invokeMethod(m_view, "setMinMaxDate", Q_ARG(QVariant, mind), Q_ARG(QVariant, maxd));
    QMetaObject::invokeMethod(m_view, "setMinMaxValue", Q_ARG(QVariant, min), Q_ARG(QVariant, max));
    
}

void TimeGraphController::clear()
{
    m_sum.clear();
}

void TimeGraphController::setView(const QQmlApplicationEngine & engine)
{
    m_view = engine.rootObjects().first()->findChild<QObject*>("timeGraph");
}
