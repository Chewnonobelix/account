#ifndef GRAPHCONTROLLER_H
#define GRAPHCONTROLLER_H


#include "abstractcontroller.h"

class GraphController: public AbstractController
{
    Q_OBJECT

private:
    QObject* m_view;
    int m_idTimer;
    QMap<QDate, Total> m_sum;

protected:
    void timerEvent(QTimerEvent *event);

public:
    GraphController();
    ~GraphController();
    void set(QObject*);
    int exec();
};

#endif // GRAPHCONTROLLER_H
