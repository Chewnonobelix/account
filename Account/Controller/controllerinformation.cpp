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
        QObject* child = m_view->findChild<QObject*>("entryEdit");
        
        connect(child, SIGNAL(s_titleChanged(QString)), this, SLOT(titleChange(QString)));
        connect(child, SIGNAL(s_valueChanged(double)), this, SLOT(valueChange(double)));
    }
}

void ControllerInformation::view(int id)
{
    m_entry = AbstractController::entry(id);
    auto child = m_view->findChild<QObject*>("entryEdit");
    QObject  *catItem;

    catItem = child->findChild<QObject*>("category");

    QStringList catList = categories(m_entry.type());
    catList<<"";

    if(catItem)
    {
        catItem->setProperty("blocked", true);
        catItem->setProperty("model", catList);
        connect(catItem, SIGNAL(s_currentTextChanged(QString)), this, SLOT(categoryChange(QString)));
        connect(catItem, SIGNAL(s_addCategory(QString)), this, SLOT(addNewCategory(QString)));
        catItem->setProperty("blocked", false);
    }

    child->setProperty("entry", QVariant::fromValue(m_entry));

    
    QObject* freqItem = m_view->findChild<QObject*>("freqCheck");
    
    if(freqItem)
    {
        connect(freqItem, SIGNAL(s_check(bool)), this, SLOT(enableFreq(bool)));
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

void ControllerInformation::setControllerFrequency(ControllerFrequency * cf)
{
    m_controllerFrequency = cf;
}

void ControllerInformation::enableFreq(bool enable)
{
    qDebug()<<"Enable freq"<<enable;
    if(enable)
    {
        Frequency f;
        m_db->addFrequency(f);
        m_entry.setFrequency(f.id());
        m_controllerFrequency->exec();
    }
}

