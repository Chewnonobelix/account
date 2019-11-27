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
        auto members = ce.members();
        auto n = "member" + QString::number(members.size());
        members<<n;
        
        for(auto it: members)
        {
            Entry e;
            Information i;
            i.setTitle("Entry " + n);
            e.setDate(QDate::currentDate());
            e.setInfo(i);
            e.setValue(3.62);
            ce.addEntries(it, e);
        }
        m_db->updateCommon(ce);
        m_view->setProperty("model", QVariant::fromValue(ce));
    }

    return 0;
}
