#ifndef CONTROLLERBUDGET_H
#define CONTROLLERBUDGET_H

#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlContext>
#include "../core/abstractcontroller.h"
#include "Model/budget.h"
#include "featurebuilder.h"
#include "filler.h"

class ControllerBudget: public AbstractController, public FeatureBuilder
{
    Q_OBJECT
    
private:    
    QObject* m_view, *m_referenceView, *m_quickView, *m_cal;
    QMap<QString, QObject*> m_views;
    QMap<QString, Budget> m_budgets;
    QDate m_currentDate;
    QString m_selected;
    Filler<QString, Budget> m_filler;
        
public:
    ControllerBudget();
    ControllerBudget(const ControllerBudget &);
    
    ~ControllerBudget() = default;
    bool removeFrom(QUuid);

    int exec();
    void openManager();
    void reload();
    void setQuickView(QObject* );
    void setManager(QObject*);
    void setCalendar(QObject*);
    
    QSharedPointer<FeatureBuilder> build(QQmlApplicationEngine *, QObject *);
    QString displayText() const;
    QString baseText() const;
    void checker() {}

public slots:
    void calDateChange();
    void show(QDate);
    void closeManager();
    void addTarget(QString);
    void removeTarget(QString, QString);
    void editReference();
    void showTarget(QString, QString, bool);

    void addBudget(QString);
    void editBudget(QString);
    void getTarget(QString);
    
    void updateEntry(QUuid);
    void changeEntry(QString, QUuid);

signals:
    void catChanged(QString);
    void addCat(QVariant);
    void clearCat();
    void addTarget2(QVariant);
    void clearTarget();
    void addSub(QVariant);
    void clearSub();
    void close();
};

Q_DECLARE_METATYPE(ControllerBudget)
#endif // CONTROLLERBUDGET_H
