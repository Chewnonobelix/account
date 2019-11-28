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
}

void ControllerCommon::removeCommon(int id)
{
    CommonExpanse ce = m_db->selectCommon()[id];
    m_db->removeCommon(ce);
    exec();
}
