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

    return 0;
}
