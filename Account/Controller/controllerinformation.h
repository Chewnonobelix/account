#ifndef CONTROLLERINFORMATION_H
#define CONTROLLERINFORMATION_H

#include <QObject>
#include <QDebug>
#include "Model/entry.h"
#include "Model/information.h"
#include "abstractcontroller.h"

class ControllerInformation: public QObject
{
    Q_OBJECT

private:
    Entry m_entry;
    QObject* m_view;

public:
    ControllerInformation();
    ~ControllerInformation();

    void show();
    void set(Entry, QObject*);

public slots:
    void labelEdit(QString);
    void estimatedEdit(bool);
    void addCategory(QString);

signals:
    void s_update(const Entry&);
    void s_addCategory(QString, QString);

};

#endif // CONTROLLERINFORMATION_H
