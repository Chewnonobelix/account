#include "controllercommon.h"

int ControllerCommon::exec()
{    
    if(m_db->selectCommon().isEmpty())
    {
        CommonExpanse e;
        e.setBegin(QDate::currentDate());
        e.setTitle("Test common expanse");
        e.setIsClose(false);
        
        m_db->addCommon(e);
    }
    

    if(!m_db->selectCommon().isEmpty())
    {
        CommonExpanse ce = m_db->selectCommon().first();
        m_view->setProperty("model", QVariant::fromValue(ce));
    }

    QObject* checker = m_view->findChild<QObject*>("close");
    if(checker)
        connect(checker, SIGNAL(s_checked(bool)), this, SLOT(closeCommon(bool)));
    
    return 0;
}

void ControllerCommon::closeCommon(bool isClose)
{
    CommonExpanse ce = m_db->selectCommon().first();
    ce.setIsClose(isClose);
    m_db->updateCommon(ce);
    ce.equilibrate();
}
