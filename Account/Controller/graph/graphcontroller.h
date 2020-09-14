#ifndef TimeGraphController_H
#define TimeGraphController_H

#include "../core/abstractcontroller.h"
#include "InterfaceGraph.h"
#include "Model/accountglobal.h"
#include <QQmlContext>
#include <QThread>

class TimeGraphController: public AbstractController, public InterfaceGraph
{
    Q_OBJECT
public:

private:
    QMap<QDate, Total> m_sum;

    QThread m_thread;
    Account::Granularity m_gran;
    int m_current;

public:
    TimeGraphController();
    ~TimeGraphController();

    int exec() override;

    void add(const Entry &) override;
    void clear() override;
    void setView(const QQmlApplicationEngine &) override;
    void update() override;
    void setGran(Account::Granularity) override;

signals:
    void clearView();
    void granChanged(QString);
    void dateChanged(QDate, QDate);
    void bornValueChanged(double, double);
    void appendMain(QDate, double);
    void appendEstimated(QDate, double);
};

#endif // TimeGraphController_H
