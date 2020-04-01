#ifndef CONTROLLERBUDGET_H
#define CONTROLLERBUDGET_H

#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>

#include "Model/budget.h"
#include "abstractcontroller.h"
#include "featurebuilder.h"
#include "filler.h"

class ControllerBudget: public AbstractController, public FeatureBuilder
{
    Q_OBJECT
    
private:
    QQmlApplicationEngine m_eng;
    
    QObject* m_view, *m_referenceView, *m_quickView, *m_cal;
    QMap<QString, QObject*> m_views;
    QMap<QString, Budget> m_budgets;
    QDate m_currentDate;
    QString m_selected;
    Filler<QString, Budget> m_filler;
        
public:
    ControllerBudget();
    ControllerBudget(const ControllerBudget &);
    
    ~ControllerBudget();
    bool removeFrom(int);

    int exec();
    void openManager();
    void reload();
    void setQuickView(QObject* );
    void setManager(QObject*);
    void setCalendar(QObject*);
    
    QSharedPointer<FeatureBuilder> build(QQmlApplicationEngine *, QObject *, QList<AbstractController *>);
    QString displayText() const;
    QString baseText() const;
    
public slots:
    void calDateChange();
    void open(QString);
    void show(QDate);
    void closeManager();
    void addTarget(QString);
    void removeTarget(QString, QString);
    void editReference();
    void showTarget(QString, QString, bool);

    void addBudget(QString);
    void editBudget(QString);
    void getTarget(QString);

    void changeFrequency(QString, int);
    
    void updateEntry(int);
    void changeEntry(QString, int);
};

Q_DECLARE_METATYPE(ControllerBudget)
#endif // CONTROLLERBUDGET_H
