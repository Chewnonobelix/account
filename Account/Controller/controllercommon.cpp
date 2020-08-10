#include "controllercommon.h"

int ControllerCommon::exec()
{
    CommonExpanse t; 
    if(!m_view->findChild<QObject*>("common")->property("model").isNull())
        t = m_view->findChild<QObject*>("common")->property("model").value<CommonExpanse>();
    
    QVariantList model;
    
    int index = -1;
    for(auto it: m_db->selectCommon())
    {
        if(t.id() == it.id())
        {
            index = model.size();
            m_view->findChild<QObject*>("common")->setProperty("model", QVariant::fromValue(it));
        }
        
        model<<QVariant::fromValue(it);
    }
    
    QObject* list = m_view->findChild<QObject*>("listCommon");
    
    if(list)
    {
        list->setProperty("model", model);
        list->setProperty("currentIndex", index);
    }
    
    
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
    
    QObject* remove = m_view->findChild<QObject*>("common")->findChild<QObject*>("remove");
    
    if(remove)
        connect(remove, SIGNAL(s_remove()), this, SLOT(removeCommonEntry()));
    
    
}

void ControllerCommon::closeCommon(bool isClose)
{
    CommonExpanse ce = m_view->findChild<QObject*>("common")->property("model").value<CommonExpanse>();
    ce.setIsClose(isClose);
    m_db->updateCommon(ce);
    ce.equilibrate();
    
    m_view->findChild<QObject*>("common")->setProperty("model", QVariant::fromValue(ce));
}

void ControllerCommon::addCommon(QString name)
{    
    CommonExpanse ce;
    ce.setTitle(name);
    ce.setBegin(QDate::currentDate());
    ce.setIsClose(false);
    m_db->addCommon(ce);
    exec();
}

void ControllerCommon::removeCommon(QVariant id)
{
    CommonExpanse ce = m_db->selectCommon()[id.toUuid()];
    m_db->removeCommon(ce);
    exec();
}

void ControllerCommon::addCommonEntry()
{
    CommonExpanse ce = m_view->findChild<QObject*>("common")->property("model").value<CommonExpanse>();
    QObject* pop = m_view->findChild<QObject*>("commonAdding");
    
    Entry e; Information i;
    i.setTitle(pop->property("v_title").toString());
    e.setDate(QDate::fromString(pop->property("v_date").toString(), "dd-MM-yyyy"));
    e.setValue(pop->property("v_val").toDouble());
    e.setType(pop->property("v_type").toString());
    e.setInfo(i);
    
    ce.addEntries(pop->property("v_member").toString().toLower(), e);
    m_db->updateCommon(ce);
    exec();
}

void ControllerCommon::removeCommonEntry()
{
    CommonExpanse ce = m_view->findChild<QObject*>("common")->property("model").value<CommonExpanse>();
    
    Entry e;
    QString member;
    if(!m_view->findChild<QObject*>("common")->findChild<QObject*>("remove")->property("currentModel").isNull())
    {
        e = m_view->findChild<QObject*>("common")->findChild<QObject*>("remove")->property("currentModel").value<Entry>();
        member = m_view->findChild<QObject*>("common")->findChild<QObject*>("remove")->property("currentMember").toString();
        
        m_db->updateCommon(ce);
    }
    
    exec();
}

QSharedPointer<FeatureBuilder> ControllerCommon::build(QQmlApplicationEngine * engine, QObject * root, QList<AbstractController *> controllers)
{
    Q_UNUSED(controllers)
    Q_UNUSED(root)

    auto common = QSharedPointer<ControllerCommon>::create();
    
    QQmlComponent commonComp(engine, QUrl("qrc:/CommonExpanse/CommonExpanseManager.qml"));
    QObject* commonManager = commonComp.create();
    
    common->m_view = commonManager;
    common->init();
    
    QObject* commonpop = commonManager->findChild<QObject*>("popAddCommon");
    if(commonpop)
        connect(commonpop, SIGNAL(s_accepted(QString)), common.data(), SLOT(addCommon(QString)));
    
    QObject* removeCommon = commonManager->findChild<QObject*>("removeCommon");
    
    if(removeCommon)
        connect(removeCommon, SIGNAL(s_remove(QVariant)), common.data(), SLOT(removeCommon(QVariant)));
    
    connect(m_db, &InterfaceDataSave::s_updateCommon, common.data(), &ControllerCommon::exec);
    common->view = commonManager;
    
    return common;
}

QString ControllerCommon::displayText() const
{
    return tr("Common Expanse");
}

QString ControllerCommon::baseText() const
{
    return "CommonExpanseFeature";
}
