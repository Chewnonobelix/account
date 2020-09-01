#ifndef CONTROLLERPIEGRAPH_H
#define CONTROLLERPIEGRAPH_H

#include "../core/abstractcontroller.h"
#include "InterfaceGraph.h"
#include <QDebug>
#include <QQmlApplicationEngine>

class ControllerPieGraph: public AbstractController, public InterfaceGraph
{
    Q_OBJECT
private:
    QObject* m_income = nullptr, *m_outcome = nullptr, *m_view;

    Account::Granularity m_gran = Account::Granularity::Month;

    QMap<QString, QMap<QString, Total>> m_entries;

public:
    ControllerPieGraph(QObject* = nullptr);
        
    void clear() override;
    void setDate(QDate) override;
    void add(const Entry &) override;
    void setGran(Account::Granularity) override;
    void setView(const QQmlApplicationEngine &) override;
    void update() override;

public slots:
    int exec() override;
};

#endif // CONTROLLERPIEGRAPH_H
