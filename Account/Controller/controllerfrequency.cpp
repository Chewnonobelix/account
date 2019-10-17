#include "controllerfrequency.h"

ControllerFrequency::ControllerFrequency()
{
    m_eng.load(QUrl(QStringLiteral("qrc:/Frequency/Generate.qml")));
    m_generate = m_eng.rootObjects().first();
    m_eng.load(QUrl(QStringLiteral("qrc:/Frequency/FrequencyManager.qml")));
    
    m_manager = m_eng.rootObjects().last();
    
    
    connect(m_generate, SLOT(s_generate(QDate, QDate)), this, SIGNAL(generate(QDate, QDate)));
    
    QObject* add, *remove, *ref, *type;
    
    add = m_manager->findChild<QObject*>("addFreq");
    remove = m_manager->findChild<QObject*>("removeFreq");
    
    if(add)
        connect(add, SIGNAL(s_addFrequency()), this, SLOT(addFrequency()));
    if(remove)
        connect(remove, SIGNAL(s_removeFrequency(int)), this, SLOT(removeFrequency(int)));
    
    QObject* cat = m_manager->findChild<QObject*>("category");
    
    if(cat)
        connect(cat, SIGNAL(s_addCategory(QString)), this, SLOT(addNewCategory(QString)));
    
    
    ref = m_manager->findChild<QObject*>("ref");
    
    if(ref)
    {
        connect(ref, SIGNAL(valueChanged(int, double)), this, SLOT(updateFreqValue(int,double)));
        connect(ref, SIGNAL(titleChanged(int, QString)), this, SLOT(updateFreqName(int,QString)));
        connect(ref, SIGNAL(catChanged(int, QString)), this, SLOT(updateFreqCat(int,QString)));
    }
    
    type = m_manager->findChild<QObject*>("type");
    if(type)
        connect(type, SIGNAL(s_updateType(int, QString)), this, SLOT(updateFreqType(int,QString)));
}

void ControllerFrequency::addEntry(int e)
{    
    if(m_freqs.contains(entry(e).frequency()))
        m_freqs[entry(e).frequency()]<<entry(e);
}

void ControllerFrequency::loadCat()
{
    auto cat = m_db->selectCategory();
    QStringList income = cat.values("income");
    income<<"";
    QStringList outcome = cat.values("outcome");
    outcome<<"";
    
    auto ee = m_manager->findChild<QObject*>("ref");
    ee->setProperty("incomeList", income);
    ee->setProperty("outcomeList", outcome);    
}

int ControllerFrequency::exec()
{
    auto freqs = m_db->selectFrequency();
    
    m_freqs.clear();
    for(auto it: freqs)
        m_freqs[it.id()] = it;
    
    loadCat();
    
    m_model.clear();
    
    for(auto it = m_freqs.begin(); it != m_freqs.end(); it++)
        m_model<<QVariant::fromValue(*it);
    
    QObject* model = m_manager->findChild<QObject*>("frequencyList");
    model->setProperty("model", m_model);    
    
    return 0;
}

void ControllerFrequency::generate(QDate begin, QDate end)
{
    QDate it = begin;
    Account::FrequencyEnum freq = Account::FrequencyEnum::unique;
    int freqId, freqGroup = 0;
    
    freqId = m_generate->property("freqId").toInt();
    freqGroup = m_generate->property("freqGroup").toInt();
    
    m_freqs[freqId].setNbGroup(freqGroup);
    
    Entry ref = m_freqs[freqId].referenceEntry();
    
    do
    {
        auto n = m_freqs[freqId].clone(ref);
        n.setMetadata("freqGroup", freqGroup);
        it = n.date();
        AbstractController::addEntry(n);
        
    }
    while(freq != Account::FrequencyEnum::unique && it <= end);
    
    QMetaObject::invokeMethod(m_generate, "close");
    exec();
}

void ControllerFrequency::openGenerate(int id)
{
    m_generate->setProperty("freqId", id);
    m_generate->setProperty("freqGroup", m_freqs[id].nbGroup() + 1);
    
    QMetaObject::invokeMethod(m_generate, "open");
}

void ControllerFrequency::openManager()
{
    exec();
    
    
    QMetaObject::invokeMethod(m_manager, "show");    
}

void ControllerFrequency::closeManager()
{
    QMetaObject::invokeMethod(m_manager, "close");
}

void ControllerFrequency::addFrequency()
{
    Frequency f;
    m_db->addFrequency(f);
    openManager();
}

void ControllerFrequency::removeFrequency(int id)
{
    m_db->removeFrequency(m_freqs[id]);
    openManager();
}

void ControllerFrequency::addNewCategory(QString cat)
{
    QObject* ref = m_manager->findChild<QObject*>("ref");
    QString type = ref->property("entry").value<Entry>().type();
    
    m_db->addCategory(cat, type);
    loadCat();
    int id = ref->property("entry").value<Entry>().id();
    updateFreqCat(id, cat);
}

void ControllerFrequency::updateFreqName(int id, QString name)
{
    Entry ref = m_freqs[id].referenceEntry();
    Information inf = ref.info();
    inf.setTitle(name);
    ref.setInfo(inf);
    
    m_freqs[id].setReferenceEntry(ref);
    
    m_db->updateFrequency(m_freqs[id]);
    exec();

    auto list = m_manager->findChild<QObject*>("frequencyList");
    for(int i = 0; i < m_model.size(); i++)
        if(m_model[i].value<Frequency>().id() == id)
            list->setProperty("currentIndex", i);
}

void ControllerFrequency::updateFreqValue(int id, double value)
{
    Entry ref = m_freqs[id].referenceEntry();
    ref.setValue(value);
    m_freqs[id].setReferenceEntry(ref);
    
    m_db->updateFrequency(m_freqs[id]);
    exec();
    
    auto list = m_manager->findChild<QObject*>("frequencyList");
    for(int i = 0; i < m_model.size(); i++)
        if(m_model[i].value<Frequency>().id() == id)
            list->setProperty("currentIndex", i);   
}

void ControllerFrequency::updateFreqCat(int id, QString cat)
{
    Entry ref = m_freqs[id].referenceEntry();
    Information inf = ref.info();
    inf.setCategory(cat);
    ref.setInfo(inf);
    
    m_freqs[id].setReferenceEntry(ref);
    
    m_db->updateFrequency(m_freqs[id]);
    exec();
    
    auto list = m_manager->findChild<QObject*>("frequencyList");
    for(int i = 0; i < m_model.size(); i++)
        if(m_model[i].value<Frequency>().id() == id)
            list->setProperty("currentIndex", i);
    
}

void ControllerFrequency::updateFreqType(int id, QString type)
{
    Entry ref = m_freqs[id].referenceEntry();
    ref.setType(type);
    Information in = ref.info();
    in.setCategory("");
    ref.setInfo(in);
    m_freqs[id].setReferenceEntry(ref);
    
    m_db->updateFrequency(m_freqs[id]);
    exec();
    
    auto list = m_manager->findChild<QObject*>("frequencyList");
    for(int i = 0; i < m_model.size(); i++)
        if(m_model[i].value<Frequency>().id() == id)
            list->setProperty("currentIndex", i);   
}

