#include "graphcontroller.h"

GraphController::GraphController(): AbstractController(), m_idTimer(0), m_view(nullptr)
{

}

GraphController::~GraphController()
{
    if(m_idTimer != 0)
        killTimer(m_idTimer);
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

    double minVal = 1000000000, maxVal = -1000000000;
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

        if(t.value() < minVal)
        {
            minVal = t.value();
            minVal = minVal < 0 ? minVal + minVal*0.05: minVal - minVal * .05;
        }

        if(t.value() > maxVal)
        {
            maxVal = t.value();
            maxVal = maxVal > 0 ? maxVal + maxVal*0.05: maxVal - maxVal * .05;
        }

        m_sum[t.date()] = t;
    }

    QMetaObject::invokeMethod(m_view, "setMinMaxDate", Q_ARG(QVariant, minDate), Q_ARG(QVariant, maxDate));
    QMetaObject::invokeMethod(m_view, "setMinMaxValue", Q_ARG(QVariant, minVal), Q_ARG(QVariant, maxVal));


    for(auto it = sum.begin(); it != sum.end(); it++)
    {
        if(it->second && m_view->property("estimatedCount").toInt() == 0 && it != sum.begin())
            QMetaObject::invokeMethod(m_view, "addDataEstimated", Q_ARG(QVariant,(it-1)->first.date()), Q_ARG(QVariant, (it-1)->first.value()));
        if(it->second)
            QMetaObject::invokeMethod(m_view, "addDataEstimated", Q_ARG(QVariant,it->first.date()), Q_ARG(QVariant, it->first.value()));
        else
            QMetaObject::invokeMethod(m_view, "addDataMain", Q_ARG(QVariant,it->first.date()), Q_ARG(QVariant, it->first.value()));

    }

    emit s_sum();

    return 0;
}
