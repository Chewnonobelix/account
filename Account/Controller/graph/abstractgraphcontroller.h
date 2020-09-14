#ifndef ABSTRACTGRAPHCONTROLLER_H
#define ABSTRACTGRAPHCONTROLLER_H

#include "../core/abstractcontroller.h"
#include "InterfaceGraph.h"
#include "controllerpiegraph.h"
#include "graphcontroller.h"
#include <QQmlApplicationEngine>

class AbstractGraphController : public AbstractController
{
    Q_OBJECT
private:
    Account::Granularity m_currentGran = Account::Granularity::Month;
    QDate m_currentDate = QDate::currentDate();
    QList<QSharedPointer<InterfaceGraph>> m_graphList;
    QObject* m_view = nullptr;
    
public:
    AbstractGraphController();
    ~AbstractGraphController();
    void set(const QQmlApplicationEngine&);
    
public slots:
    void change(int);
    void increment(int = 0);
    int exec() override;

signals:
    void granChanged(Account::Granularity);
    void dateChanged(QDate);
    void clear();
    void okPrevChanged(bool);
    void okNextChanged(bool);
};

#endif // ABSTRACTGRAPHCONTROLLER_H
