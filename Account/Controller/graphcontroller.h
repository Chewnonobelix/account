#ifndef TimeGraphController_H
#define TimeGraphController_H

#include <QThread>
#include "abstractcontroller.h"
#include "Model/accountglobal.h"
#include "InterfaceGraph.h"

class TimeGraphController: public AbstractController, public InterfaceGraph
{
    Q_OBJECT
public:

private:
    QObject* m_view;

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
    void setDate(QDate) override;
    void setGran(Account::Granularity) override;
    void setView(const QQmlApplicationEngine &) override;
    void update() override;
};

#endif // TimeGraphController_H
