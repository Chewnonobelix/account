#ifndef CONTROLLERINFORMATION_H
#define CONTROLLERINFORMATION_H

#include <QObject>
#include <QDebug>
#include "Model/entry.h"
#include "Model/information.h"
#include "abstractcontroller.h"
#include "controllerfrequency.h"

class ControllerInformation: public AbstractController
{
    Q_OBJECT

private:
    Entry m_entry;
    QObject* m_view;
    ControllerFrequency* m_controllerFrequency;
    
public:
    ControllerInformation();
    ~ControllerInformation();

    int exec();

    void configure(QObject*);
    void view(int);
    void setControllerFrequency(ControllerFrequency*);
    
public slots:
    void titleChange(QString);
    void valueChange(double);
    void categoryChange(QString);
    void addNewCategory(QString);
    void enableFreq(bool);
    
signals:
    void s_update(int);
    void s_changeCat(QString, int);
};

#endif // CONTROLLERINFORMATION_H
