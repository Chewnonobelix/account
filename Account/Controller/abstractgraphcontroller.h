#ifndef ABSTRACTGRAPHCONTROLLER_H
#define ABSTRACTGRAPHCONTROLLER_H

#include "abstractcontroller.h"
#include "InterfaceGraph.h"

class AbstractGraphController : public AbstractController
{
    Q_OBJECT
private:
    Account::Granularity m_currentGran = Account::Month;
    QDate m_currentDate = QDate::currentDate();
    QList<QSharedPointer<InterfaceGraph>> m_graphList;

public:
    AbstractGraphController() = default;
    ~AbstractGraphController() = default;

public slots:
    void change(int);
    void increment(int = 0);
    int exec() override;
};

#endif // ABSTRACTGRAPHCONTROLLER_H
