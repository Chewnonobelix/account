#include "graphcontroller.h"

GraphController::GraphController(): AbstractController(), m_view(nullptr), m_gran(month)
{

}

GraphController::~GraphController()
{
}


void GraphController::set(QObject * view)
{
    m_view = view;

    if(m_view)
    {
        connect(m_view, SIGNAL(s_increment(int)), this, SLOT(increment(int)));
        connect(m_view, SIGNAL(s_zoom(int)), this, SLOT(change(int)));
    }
}

QMap<QDate, Total> GraphController::sum() const
{
    return m_sum;
}

int GraphController::exec()
{
    m_sum.clear();

    if(!m_view)
        return -1;

    QMetaObject::invokeMethod(m_view, "clear");
    auto dateList = allDate();
    QList<QDate> keysT;
    QDate minDate , maxDate;

    if(!dateList.isEmpty())
    {
        minDate = dateList.first();
        maxDate = dateList.last();
    }

    QDate cd = minDate;
    keysT<<minDate;
    while(cd != maxDate)
    {
        cd = cd.addDays(1);
        keysT<<cd;
    }

    Total t;

    QList<QPair<Total, bool>> sum;
    for(auto it: keysT)
    {
        auto l = entries(it);
        if(l.isEmpty())
        {
            Entry e;
            e.setDate(it);
            e.setValue(0);
            Information i;
            i.setEstimated(it > QDate::currentDate());
            e.setInfo(i);
            l<<e;
        }


        for(auto it2: l)
        {
            t = t + it2;
        }

        sum.append(qMakePair(t, l.first().info().estimated()));

        m_sum[t.date()] = t;
    }



    increment();

    emit s_sum();
    return 0;
}

void GraphController::change(int nGranularity)
{
    m_gran = (granularity)((m_gran + nGranularity + 3)%3);
    increment();
}

void GraphController::increment(int inc)
{
    QMetaObject::invokeMethod(m_view, "clear");

    QMap<QDate, Total> ret;
    int cMonth = m_view->property("month").toInt();
    int cYear = m_view->property("years").toInt();
    QDate itDate, lastDay;
    QSharedPointer<int> minVal, maxVal;

    auto setMin = [&](double val) {
        if(minVal.isNull())
        {
            minVal.reset(new int);
            *minVal = val;
        }
        else if(val < *minVal)
        {
            *minVal = val;
        }
    };

    auto setMax = [&](double val) {
        if(minVal.isNull())
        {
            maxVal.reset(new int);
            *maxVal = val;
        }
        else if(val > *maxVal)
        {
            *maxVal = val;
        }
    };

    m_view->setProperty("okPrev", (m_gran != over));
    m_view->setProperty("okNext", (m_gran != over));

    switch(m_gran)
    {
    case month:
        cMonth += inc;

        if(cMonth == 0)
        {
            cMonth = 12;
            cYear --;
        }
        else if(cMonth == 13)
        {
            cMonth = 1;
            cYear ++;
        }

        m_view->setProperty("granularity", tr("one month"));

        for(auto it: m_sum)
        {
            if(it.date().month() == cMonth && it.date().year() == cYear)
            {
                ret[it.date()] = it;
                setMax(it.value());
                setMin(it.value());
            }
        }
        break;

    case year:
        cYear += inc;
        itDate.setDate(cYear, 1, 1);
        if(m_sum.contains(itDate))
            ret[itDate] = m_sum[itDate];
        itDate = itDate.addDays(30);
        m_view->setProperty("granularity", tr("one year"));

        if(!m_sum.contains(itDate.addMonths(-1)))
            ret[m_sum.first().date()] = m_sum.first();
        for(int i = 0; i < 12; i++)
        {
            if(m_sum.contains(itDate))
            {
                ret[itDate] = m_sum[itDate];
                setMax(m_sum[itDate].value());
                setMin(m_sum[itDate].value());
            }
            else
            {
                QDate wtf;
                wtf.setDate(itDate.year(), itDate.month()-1, 1);
                if(m_sum.contains(wtf))
                    ret[m_sum.last().date()] = m_sum.last();
            }
            itDate = itDate.addMonths(1);
            itDate = itDate.addDays(itDate.daysInMonth() - itDate.day());
        }
        break;

    case over:
        itDate = m_sum.first().date();
        ret[itDate] = m_sum[itDate];
        itDate = itDate.addDays(itDate.daysInMonth() - itDate.day());
        lastDay = m_sum.last().date();
        m_view->setProperty("granularity", tr("all years"));
        while(itDate < lastDay)
        {
            if(m_sum.contains(itDate))
            {
                ret[itDate] = m_sum[itDate];
                setMax(m_sum[itDate].value());
                setMin(m_sum[itDate].value());
            }

            itDate = itDate.addMonths(1);
            itDate = itDate.addDays(itDate.daysInMonth() - itDate.day());

        }

        if(!ret.contains(lastDay))
        {
            ret[lastDay] = m_sum[lastDay];
            setMax(m_sum[lastDay].value());
            setMin(m_sum[lastDay].value());
        }
        break;
    }

    m_view->setProperty("month", cMonth);
    m_view->setProperty("years", cYear);

    QDate minDate, maxDate;
    if(!ret.isEmpty())
    {
        minDate = ret.first().date();
        maxDate = ret.last().date();
    }

    if(!ret.isEmpty() && !m_sum.isEmpty())
    {
        m_view->setProperty("okPrev", !(ret.first() == m_sum.first()));
        m_view->setProperty("okNext", !(ret.last() == m_sum.last()));
    }

    if(!maxVal.isNull())
    {
        *maxVal = *maxVal > 0 ? *maxVal * 1.05 : *maxVal * .95;
        *minVal = *minVal < 0 ? *minVal * 1.05 : *minVal * .95;


        QMetaObject::invokeMethod(m_view, "setMinMaxDate", Q_ARG(QVariant, minDate), Q_ARG(QVariant, maxDate));
        QMetaObject::invokeMethod(m_view, "setMinMaxValue", Q_ARG(QVariant, *minVal), Q_ARG(QVariant, *maxVal));
    }

    for(auto it = ret.begin(); it != ret.end(); it++)
    {
        if(it.value().date() > QDate::currentDate() && m_view->property("estimatedCount").toInt() == 0 && it != ret.begin())
            QMetaObject::invokeMethod(m_view, "addDataEstimated", Q_ARG(QVariant,(it-1).value().date()), Q_ARG(QVariant, (it-1).value().value()));
        if(it.value().date() > QDate::currentDate())
            QMetaObject::invokeMethod(m_view, "addDataEstimated", Q_ARG(QVariant,it.value().date()), Q_ARG(QVariant, it.value().value()));
        else
            QMetaObject::invokeMethod(m_view, "addDataMain", Q_ARG(QVariant,it.value().date()), Q_ARG(QVariant, it.value().value()));

    }
}
