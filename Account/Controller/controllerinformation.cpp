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
    }
}

void ControllerInformation::set(Entry e, QObject* v)
{
    m_entry = e;
    m_view = v;

    connect(v, SIGNAL(s_titleChanged(QString)), this, SLOT(labelEdit(QString)));
    connect(v, SIGNAL(s_estimatedChanged(bool)), this, SLOT(estimatedEdit(bool)));
    show();
}
