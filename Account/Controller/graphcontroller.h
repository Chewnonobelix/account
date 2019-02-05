#ifndef GRAPHCONTROLLER_H
#define GRAPHCONTROLLER_H

#include <QThread>
#include "abstractcontroller.h"

class GraphController: public AbstractController
{
    Q_OBJECT

private:
    QObject* m_view;
    int m_idTimer;
    QMap<QDate, Total> m_sum;

    QThread m_thread;

protected:
    void timerEvent(QTimerEvent *event);

public:
    GraphController();
    ~GraphController();
    void set(QObject*);
    int exec();

    QMap<QDate, Total> sum() const;

signals:
    void s_sum();
};

#endif // GRAPHCONTROLLER_H
