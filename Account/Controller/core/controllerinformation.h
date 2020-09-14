#ifndef CONTROLLERINFORMATION_H
#define CONTROLLERINFORMATION_H

#include "../features/controllerfrequency.h"
#include "Model/entry.h"
#include "Model/information.h"
#include "abstractcontroller.h"
#include <QDebug>
#include <QObject>

class ControllerInformation: public AbstractController
{
    Q_OBJECT

private:
    Entry m_entry;
    int m_currentPage = 1, m_maxPage = 1;

public:
    ControllerInformation() = default;
    ~ControllerInformation() = default;

    int exec();

    void view(QUuid);

public slots:
    void titleChange(QString);
    void valueChange(double);
    void categoryChange(QString);
    void addNewCategory(QString);
    void pageChange();
    void supportChange(int);

signals:
    void s_update(QUuid);
    void s_changeCat(QString, QUuid);
    void s_exec();

    void frequencyVisible(bool);

    void maxPageChanged(int);
    void currentPageChanged(int);
    void pageModel(QVariant);

    void view(QVariant);
    void catList(QVariant);
};

#endif // CONTROLLERINFORMATION_H
