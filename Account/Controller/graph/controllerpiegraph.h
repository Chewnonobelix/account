#ifndef CONTROLLERPIEGRAPH_H
#define CONTROLLERPIEGRAPH_H

#include "../core/abstractcontroller.h"
#include "InterfaceGraph.h"
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>

class ControllerPieGraph: public AbstractController, public InterfaceGraph
{
    Q_OBJECT
private:
    QMap<QString, QMap<QString, Total>> m_entries;

public:
    ControllerPieGraph(QObject * = nullptr);

    void clear() override;
    void add(const Entry &) override;
    void setView(const QQmlApplicationEngine &) override;
    void update() override;
    void setGran(Account::Granularity) override;

public slots:
    int exec() override;

signals:
    void append(QString, QString, double);
    void clearView();
};

#endif // CONTROLLERPIEGRAPH_H
