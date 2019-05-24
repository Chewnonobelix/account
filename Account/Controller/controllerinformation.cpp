#include "controllerinformation.h"

ControllerInformation::ControllerInformation(): AbstractController()
{
}

ControllerInformation::~ControllerInformation()
{
}

int ControllerInformation::exec()
{
    return 0;
}

void ControllerInformation::configure(QObject * view)
{
    m_view = view;

    if(m_view)
    {
        connect(m_view, SIGNAL(s_titleChanged(QString)), this, SLOT(titleChange(QString)));
        connect(m_view, SIGNAL(s_valueChanged(double)), this, SLOT(valueChange(double)));
    }
}

void ControllerInformation::view(int id)
{
    m_entry = AbstractController::entry(id);

    QObject * entryItem, * infoItem, *catItem;
    entryItem = m_view->findChild<QObject*>("entry");
    infoItem = m_view->findChild<QObject*>("infoModel");
    catItem = m_view->findChild<QObject*>("category");

    if(entryItem)
    {
        entryItem->setProperty("id", id);
        entryItem->setProperty("value", m_entry.value());
    }

    if(infoItem)
    {
        infoItem->setProperty("estimated", m_entry.info().estimated());
        infoItem->setProperty("title", m_entry.info().title());
        infoItem->setProperty("type", m_entry.info().category());
    }

    QStringList catList = categories(m_entry.type());
    catList<<"";
    if(catItem)
    {
        catItem->setProperty("model", catList);
        connect(catItem, SIGNAL(s_currentTextChanged(QString)), this, SLOT(categoryChange(QString)));
        connect(catItem, SIGNAL(s_addCategory(QString)), this, SLOT(addNewCategory(QString)));
    }
}

void ControllerInformation::titleChange(QString title)
{
    Information info = m_entry.info();
    info.setTitle(title);
    m_entry.setInfo(info);

    updateEntry(m_entry);
}

void ControllerInformation::valueChange(double value)
{
    m_entry.setValue(value);

    updateEntry(m_entry);
    emit s_update(m_entry.id());
}

void ControllerInformation::categoryChange(QString cat)
{
    Information info = m_entry.info();
    QString old = info.category();   
    info.setCategory(cat);
    m_entry.setInfo(info);

    updateEntry(m_entry);
    emit s_changeCat(old, m_entry.id());
}

void ControllerInformation::addNewCategory(QString cat)
{
    addCategory(cat, m_entry.type());
    categoryChange(cat);
    view(m_entry.id());
}


