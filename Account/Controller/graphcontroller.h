#ifndef TimeGraphController_H
#define TimeGraphController_H

#include <QThread>
#include "abstractcontroller.h"

class TimeGraphController: public AbstractController
{
    Q_OBJECT
public:
    enum granularity {month, year, over};

private:
    QObject* m_view;

    QMap<QDate, Total> m_sum;

    QThread m_thread;
    granularity m_gran;
    int m_current;

public:
    TimeGraphController();
    ~TimeGraphController();
    void set(QObject*);
    int exec();


    QMap<QDate, Total> sum() const;

public slots:
    void change(int);
    void increment(int = 0);

signals:
    void s_sum();
};

#endif // TimeGraphController_H
