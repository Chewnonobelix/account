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
    minDate = dateList.first(); maxDate = dateList.last();

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
    m_gran = (granularity)nGranularity;
    increment();
}

void GraphController::increment()
{
    QMap<QDate, Total> ret;
    int cMonth = m_view->property("month").toInt();
    int cYear = m_view->property("years").toInt();
    QDate itDate, lastDay;
    QSharedPointer<int> minVal, maxVal;

    qDebug()<<"increment"<<m_sum.size()<<m_gran<<cMonth<<cYear;

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

    switch(m_gran)
    {
    case month:
        for(auto it: m_sum)
        {
            if(it.date().month() == cMonth && it.date().year() == cYear)
            {
                qDebug()<<it.date();
                ret[it.date()] = it;
                setMax(it.value());
                setMin(it.value());
            }
        }
        break;
    case year:
        itDate.setDate(cYear, 1, 31);

        for(int i = 0; i < 12; i++)
        {
            qDebug()<<itDate;
            ret[itDate] = m_sum[itDate];
            setMax(m_sum[itDate].value());
            setMin(m_sum[itDate].value());
        }
        break;
    case over:
        itDate.setDate(cYear, cMonth, 1);
        itDate = itDate.addDays(itDate.daysInMonth() - 1);
        lastDay = m_sum.last().date();

        while(itDate < lastDay)
        {
            qDebug()<<"Over"<<itDate;
            ret[itDate] = m_sum[itDate];
            setMax(m_sum[itDate].value());
            setMin(m_sum[itDate].value());
        }

        if(!ret.contains(lastDay))
        {
            ret[lastDay] = m_sum[lastDay];
            setMax(m_sum[lastDay].value());
            setMin(m_sum[lastDay].value());
        }
        break;
    }

    QDate minDate, maxDate;
    minDate = ret.first().date();
    maxDate = ret.last().date();

    qDebug()<<*minVal<<*maxVal;

    *maxVal = *maxVal > 0 ? *maxVal * 1.05 : *maxVal * .95;
    *minVal = *minVal < 0 ? *minVal * 1.05 : *minVal * .95;

    QMetaObject::invokeMethod(m_view, "setMinMaxDate", Q_ARG(QVariant, minDate), Q_ARG(QVariant, maxDate));
    QMetaObject::invokeMethod(m_view, "setMinMaxValue", Q_ARG(QVariant, *minVal), Q_ARG(QVariant, *maxVal));

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
