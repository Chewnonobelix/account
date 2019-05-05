#ifndef CONTROLLERINFORMATION_H
#define CONTROLLERINFORMATION_H

#include <QObject>
#include <QDebug>
#include "Model/entry.h"
#include "Model/information.h"
#include "abstractcontroller.h"

class ControllerInformation: public AbstractController
{
    Q_OBJECT

private:
    Entry m_entry;
    QObject* m_view;

public:
    ControllerInformation();
    ~ControllerInformation();

    int exec();

    void configure(QObject*);
    void view(int);

public slots:
    void titleChange(QString);
    void valueChange(double);
    void categoryChange(QString);
    void addNewCategory(QString);

signals:

};

#endif // CONTROLLERINFORMATION_H
