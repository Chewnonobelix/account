#include "controllercommon.h"

int ControllerCommon::exec()
{
    CommonExpanse t;
    if (!m_currentId.isNull())
        t = db()->selectCommon()[m_currentId];

    QVariantList model;

    int index = -1;
    for(auto it: m_db->selectCommon())
    {
        if(t.id() == it.id())
        {
            index = model.size();
        }
        
        model<<QVariant::fromValue(it);
    }

    emit commonModelChanged(model);
    emit currentIndexChanged(index);

    return 0;
}

void ControllerCommon::init()
{
    exec();
}

void ControllerCommon::closeCommon(QString id, bool isClose)
{
    CommonExpanse ce = db()->selectCommon()[id];
    ce.setIsClose(isClose);
    m_db->updateCommon(ce);
    ce.equilibrate();

    exec();
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

void ControllerCommon::addCommonEntry(QVariant ref)
{
    CommonExpanse ce = db()->selectCommon()[m_currentId];

    Entry e;
    QVariantMap map = ref.toMap();

    e.setTitle(map["title"].toString());
    e.setDate(QDate::fromString(map["date"].toString(), "dd-MM-yyyy"));
    e.setValue(map["value"].toDouble());
    e.setType(map["type"].value<Account::TypeEnum>());

    ce.addEntries(map["member"].toString(), e);
    m_db->updateCommon(ce);
    exec();
}

void ControllerCommon::removeCommonEntry(QString idc, QString member, QString ide)
{
    CommonExpanse ce = db()->selectCommon()[idc];
    auto l = ce.entries().values(member);
    Entry e;
    for (auto it : l)
        if (it.id() == ide)
            e = it;

    ce.removeEntry(member, e);
    m_db->updateCommon(ce);

    exec();
}

QSharedPointer<FeatureBuilder> ControllerCommon::build(QQmlApplicationEngine * engine, QObject * root)
{
    Q_UNUSED(root)

    auto common = QSharedPointer<ControllerCommon>::create();
    auto* context = engine->rootContext();
    context->setContextProperty("_commonExpanse", common.data());
    QQmlComponent commonComp(engine, QUrl("qrc:/CommonExpanse/CommonExpanseManager.qml"));
    QObject* commonManager = commonComp.create();
    
    common->init();
        
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
