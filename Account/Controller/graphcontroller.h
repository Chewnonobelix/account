#ifndef GRAPHCONTROLLER_H
#define GRAPHCONTROLLER_H

#include <QThread>
#include "abstractcontroller.h"

class GraphController: public AbstractController
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
    GraphController();
    ~GraphController();
    void set(QObject*);
    int exec();


    QMap<QDate, Total> sum() const;

public slots:
    void change(int);
    void increment(int = 0);

signals:
    void s_sum();
};

#endif // GRAPHCONTROLLER_H
