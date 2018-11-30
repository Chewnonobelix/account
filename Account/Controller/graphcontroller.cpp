#include "graphcontroller.h"

GraphController::GraphController(): AbstractController(), m_idTimer(0)
{

}

GraphController::~GraphController()
{
    if(m_idTimer != 0)
        killTimer(m_idTimer);
}

int GraphController::exec()
{
    m_idTimer = startTimer(1000);
    return 0;
}

void GraphController::set(QObject * view)
{
    m_view = view;
}
void GraphController::timerEvent(QTimerEvent *)
{
    m_sum.clear();
    m_sumEstimated.clear();
    QMetaObject::invokeMethod(m_view, "clear");
    auto entrieslist = entries();
    QList<QDate> keysT;
    QDate minDate , maxDate;

    for(auto it: entrieslist)
    {
        if((minDate.isValid() && it.date() < minDate) || minDate.isNull())
            minDate = it.date();

        if((maxDate.isValid() && it.date() > maxDate) || maxDate.isNull())
            maxDate = it.date();
    }

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



    }

    QMetaObject::invokeMethod(m_view, "setMinMaxDate", Q_ARG(QVariant, minDate), Q_ARG(QVariant, maxDate));
    QMetaObject::invokeMethod(m_view, "setMinMaxValue", Q_ARG(QVariant, minVal), Q_ARG(QVariant, maxVal));


    for(auto it = sum.begin(); it != sum.end(); it++)
    {
        if(it->second && m_view->property("estimatedCount").toInt() == 0)
            QMetaObject::invokeMethod(m_view, "addDataEstimated", Q_ARG(QVariant,(it-1)->first.date()), Q_ARG(QVariant, (it-1)->first.value()));
        if(it->second)
            QMetaObject::invokeMethod(m_view, "addDataEstimated", Q_ARG(QVariant,it->first.date()), Q_ARG(QVariant, it->first.value()));
        else
            QMetaObject::invokeMethod(m_view, "addDataMain", Q_ARG(QVariant,it->first.date()), Q_ARG(QVariant, it->first.value()));

    }
}
