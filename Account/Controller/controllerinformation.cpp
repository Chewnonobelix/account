#include "controllerinformation.h"

ControllerInformation::ControllerInformation(): QObject(nullptr)
{
}

ControllerInformation::~ControllerInformation() {}

void ControllerInformation::labelEdit(QString l)
{
    Information i = m_entry.info();
    i.setTitle(l);

    m_entry.setInfo(i);

    show();
    s_update(m_entry);
}

void ControllerInformation::estimatedEdit(bool e)
{
    Information i = m_entry.info();
    i.setEstimated(e);

    m_entry.setInfo(i);

    show();
    s_update(m_entry);
}

void ControllerInformation::show()
{
    QObject* model =  m_view->findChild<QObject*>("entry");
    if(model)
    {
        model->setProperty("id", m_entry.id());
        model->setProperty("value", m_entry.value());
    }

    model = m_view->findChild<QObject*>("infoModel");
    if(model)
    {
        model->setProperty("estimated", m_entry.info().estimated());
        model->setProperty("title", m_entry.info().title());
        model->setProperty("type", m_entry.info().category());
    }
}

void ControllerInformation::set(Entry e, QObject* v)
{
    m_entry = e;
    m_view = v;

    QObject* combo = v->findChild<QObject*>("category");
    if(combo)
        connect(combo, SIGNAL(s_addCategory(QString)), this, SLOT(addCategory(QString)));

    connect(v, SIGNAL(s_titleChanged(QString)), this, SLOT(labelEdit(QString)));
    connect(v, SIGNAL(s_estimatedChanged(bool)), this, SLOT(estimatedEdit(bool)));
    show();
}


void ControllerInformation::addCategory(QString name)
{
    emit s_addCategory(name, m_entry.type());
}
