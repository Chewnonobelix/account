#include "controllercommon.h"

int ControllerCommon::exec()
{
    m_engine.load(QUrl(QStringLiteral("qrc:/CommonExpanse/CommonExpanseView.qml")));
    m_view = m_engine.rootObjects().first();
    
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
        m_view->setProperty("cModel", QVariant::fromValue(ce));
    }

    return 0;
}
