#include "controllercommon.h"

int ControllerCommon::exec()
{
    QVariantList model;
    for(auto it: m_db->selectCommon())
        model<<QVariant::fromValue(it);
    
    QObject* list = m_view->findChild<QObject*>("listCommon");

    if(list)
        list->setProperty("model", model);
    
    return 0;
}

void ControllerCommon::init()
{
    QObject* checker = m_view->findChild<QObject*>("close");
    if(checker)
        connect(checker, SIGNAL(s_checked(bool)), this, SLOT(closeCommon(bool)));
    
    QObject* popup = m_view->findChild<QObject*>("commonAdding");
    
    if(popup)
        connect(popup, SIGNAL(accept()), this, SLOT(addCommonEntry()));
    
}

void ControllerCommon::closeCommon(bool isClose)
{
    CommonExpanse ce = m_db->selectCommon().first();
    ce.setIsClose(isClose);
    m_db->updateCommon(ce);
    ce.equilibrate();
}

void ControllerCommon::addCommon(QString name)
{
    qDebug()<<"Add common"<<name;
    
    CommonExpanse ce;
    ce.setTitle(name);
    ce.setBegin(QDate::currentDate());
    ce.setIsClose(false);
    m_db->addCommon(ce);
    exec();
}

void ControllerCommon::removeCommon(int id)
{
    CommonExpanse ce = m_db->selectCommon()[id];
    m_db->removeCommon(ce);
    exec();
}

void ControllerCommon::addCommonEntry()
{
    CommonExpanse ce = m_view->findChild<QObject*>("common")->property("model").value<CommonExpanse>();
    QObject* pop = m_view->findChild<QObject*>("commonAdding");
    
    Entry e; Information i;
    i.setTitle(pop->property("v_title").toString());
    e.setDate(pop->property("v_date").toDate());
    e.setValue(pop->property("v_value").toDouble());
    e.setType(pop->property("v_type").toString());
    e.setInfo(i);
    
    ce.addEntries(pop->property("v_member").toString(), e);
    m_db->updateCommon(ce);
    exec();
}
