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
void GraphController::timerEvent(QTimerEvent *event)
{
    qDebug()<<"Timer event"<<event->timerId();
    m_sum.clear();
    m_sumEstimated.clear();
    QMetaObject::invokeMethod(m_view, "clear");
    auto entrieslist = entries();
    QSet<QDate> keys;

    for(auto it: entrieslist)
        keys<<it.date();

    QDate minDate, maxDate;
    double minVal = 1000000000, maxVal = -1000000000;

    for(auto it: keys)
    {
        Total t;
        Total te;

        auto l = entries(it);

        if((minDate.isValid() && it < minDate) || minDate.isNull())
            minDate = it;

        if((maxDate.isValid() && it > maxDate) || maxDate.isNull())
            maxDate = it;


        for(auto it2: l)
        {
            if(it2.info().estimated())
            {
                te = te + it2;
            }
            else
            {
                t = t+ it2;
            }
        }

        if(t.value() < minVal)
            minVal = t.value();

        if(t.value() > maxVal)
            maxVal = t.value();

        m_sum[it] = t;
        m_sumEstimated[it] = te;
    }

    QMetaObject::invokeMethod(m_view, "setMinMaxDate", Q_ARG(QVariant, minDate), Q_ARG(QVariant, maxDate));
    QMetaObject::invokeMethod(m_view, "setMinMaxValue", Q_ARG(QVariant, minVal), Q_ARG(QVariant, maxVal));


    qDebug()<<m_sum.size()<<m_sumEstimated.size();
    for(auto it: m_sum)
        QMetaObject::invokeMethod(m_view, "addDataMain", Q_ARG(QVariant,it.date()), Q_ARG(QVariant, it.value()));

    for(auto it: m_sumEstimated)
        QMetaObject::invokeMethod(m_view, "addDataEstimated", Q_ARG(QVariant,it.date()), Q_ARG(QVariant, it.value()));

}
