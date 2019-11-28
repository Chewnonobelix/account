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
